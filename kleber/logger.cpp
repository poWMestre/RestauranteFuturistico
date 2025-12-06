/*
  Class Logger
  ------------
  Implementação simples de um logger "thread-safe" em C++ usando:
   - singleton (Meyers singleton)
   - std::format / std::vformat para formatação tipo "{}"
   - std::mutex para evitar interleaving (intercalação) quando múltiplas threads escrevem ao mesmo tempo
*/

// Importação das bibliotecas da Standart Library (std).
#include <iostream>
#include <thread>
#include <mutex>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <format>
#include <random>
#include <vector>

class Logger {
public:
    // Tipos (também chamado de níveis) de log.
    enum class LogLevel {
        DEBUG, INFO, WARNING, ERROR
    };

    // 
    /**
     * Acessor estático para o singleton (Meyers Singleton).
     * A primeira chamada cria uma instância estática local e futuras chamadas retornam a mesma instância. 
     * Desde C++11 esta inicialização é segura (thread-safe) em ambientes multithreads.
     */
    static Logger &get() {
        // Aqui a instância padrão usa std::cout e um formato de tempo curto.
        static Logger instance(std::cout, "%H:%M:%S.%s");
        return instance;
    }

    /**
     * Construtor do Logger.
     * 
     * @param output referência para um std::ostream (similar a um OutputStream do Java) onde as mensagens serão escritas
     * @param date_time_format string com o formato que será passado a timeToString
     */
    explicit Logger(std::ostream &output = std::cout, std::string_view date_time_format = "%Y-%m-%d %H:%M:%S") :
            output(output), date_time_format(date_time_format) {}

    /**
     * Método de impressão bruta formatada (sem acrescentar '\n' automaticamente).
     * Template variádico aceita quaisquer argumentos compatíveis com std::vformat.
     * 
     * @param format formato da string (placeholders {})
     * @param args argumentos a serem formatados
     */
    template<typename... Args>
    void print(const std::string_view format, Args &&... args) const {
        // lock_guard garante que o mutex será desbloqueado ao sair do escopo (mesmo em caso de exceção).
        // No livro, o uso de mutex é feito pelas funções down e up. No entanto, o uso desta classe torna 
        // ainda mais prático, pois no construtor ele faz o lock (down) e no destrutor o unlock (up).
        // Além disso, evita deadlocks em caso de exceção.
        std::lock_guard<std::mutex> lock(mutex);

        // std::vformat formata a string usando o formato tipo "std::format" (placeholders {})
        output << std::vformat(format, std::make_format_args(args...));
    }

    /**
     * Método de log sem nível — prefixa a mensagem com timestamp.
     * Gera uma linha com "timestamp | mensagem\n".
     *
     * @param format format do print
     * @param args argumentos a serem formatados
     */
    template<typename... Args>
    void log(const std::string_view format, Args &&... args) const {
        // obtém o tempo atual e formata para string, depois a mensagem formatada
        print("{} | {}\n", timeToString(std::chrono::system_clock::now(), date_time_format),
              std::vformat(format, std::make_format_args(args...)));
    }

    /**
     * Método de log com nível — adiciona o nível entre colchetes:
     * "[LEVEL] timestamp | mensagem\n"
     *
     * @param level nível do log (DEBUG, INFO, WARNING, ERROR)
     * @param format format do print
     * @param args argumentos a serem formatados
     */
    template<typename... Args>
    void log(const LogLevel level, const std::string_view format, Args &&... args) const {
        print("[{}] {} | {}\n", levelToString(level), timeToString(std::chrono::system_clock::now(), date_time_format),
              std::vformat(format, std::make_format_args(args...)));
    }

    /**
     * Conforto: wrappers para os níveis mais comuns. Eles simplesmente chamam `log` com o LogLevel correspondente.
     *
     * @param format 
     * @param args 
     */
    template<typename... Args>
    void debug(const std::string_view format, Args &&... args) const {
        log(LogLevel::DEBUG, format, args...);
    }

    template<typename... Args>
    void info(const std::string_view format, Args &&... args) const {
        log(LogLevel::INFO, format, args...);
    }

    template<typename... Args>
    void warn(const std::string_view format, Args &&... args) const {
        log(LogLevel::WARNING, format, args...);
    }

    template<typename... Args>
    void error(const std::string_view format, Args &&... args) const {
        log(LogLevel::ERROR, format, args...);
    }

private:
    // Referência para o stream de saída (por exemplo std::cout).
    // Importante: garantir que o ostream referenciado viva pelo tempo de vida do Logger.
    std::ostream &output;

    // Mutex usado para proteger `output` contra escritas concorrentes.
    // Declara-se `mutable` para permitir a alteração do estado da mutex mesmo em métodos const.
    mutable std::mutex mutex;

    // String que guarda o formato de data/hora utilizado por timeToString.
    std::string date_time_format;

    /**
     * Esta função transforma um std::chrono::system_clock::time_point numa std::string,
     * usando o formato `date_time_format`.
     *
     * - Requer C++20 (std::format / std::vformat).
     * 
     * @param time_point tempo a ser formatado.
     * @param format formato de data/hora (padrão: "%Y-%m-%d %H:%M:%S").
     * @return string com a representação formatada do tempo.
     */
    static std::string timeToString(std::chrono::system_clock::time_point time_point,
                                    std::string format = "%Y-%m-%d %H:%M:%S") {
        auto show_milliseconds = false;
        if (auto pos = format.find("%s"); pos != std::string::npos) {
            format.erase(pos, 2);
            show_milliseconds = true;
        }

        auto time_t = std::chrono::system_clock::to_time_t(time_point);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), format.c_str());
        if (show_milliseconds) {
            auto duration = time_point.time_since_epoch();
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
            ss << std::setfill('0') << std::setw(3) << milliseconds.count();
        }
        return ss.str();
    }

    /**
     * Converte o enum LogLevel para uma string textual.
     * Implementado como static porque não precisa de estado da instância.
     * 
     * @param level 
     * @return std::string 
     */
    static std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:   return "DEBUG";
            case LogLevel::INFO:    return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR:   return "ERROR";
        }
        // Este return é para satisfazer todos os caminhos — com o enum atual nunca será chamado,
        // mas evita warning de compilador sobre controle de fluxo.
        return "UNKNOWN";
    }
};

// Namespace utilitário com funções inline que encaminham para o singleton Logger.
// Serve para chamar de forma mais curta: 
// - logger::print(...)
// - logger::log(...)
namespace logger {

    template<typename... Args>
    inline void print(const std::string_view format, Args &&... args) {
        // chama a instância global e delega
        Logger::get().print(format, args...);
    }

    template<typename... Args>
    inline void log(const std::string_view format, Args &&... args) {
        Logger::get().log(format, args...);
    }
}

int main() {
    Logger logger; // supondo construtor padrão já esteja ok

    auto worker = [&](int id) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(10, 50);

        for (int i = 0; i < 20; i++) {
            // std::cerr << "Thread " << id << " - mensagem " << i << "\n"; // (thread-unsafe)
            logger.log("Thread {} - mensagem {}", id, i); // (logger thread-safe)
            std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
        }
    };

    // cria várias threads que logam ao mesmo tempo
    const int num_threads = 8;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back(worker, t);
    }

    // espera todas terminarem
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "Fim do teste de logger thread-safe!\n";
    return 0;
}
