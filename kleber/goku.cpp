#include <iostream>     // std::cout, std::cerr, std::endl
#include <unistd.h>     // POSIX: pipe(), fork(), read(), write(), getpid(), close()
#include <sys/wait.h>   // wait()/macros relacionadas (não usado ativamente aqui porque SIGCHLD é ignorado)
#include <format>       // C++20: std::format
#include <string>       // std::string

/**
 * Função executada pelo processo pai (Goku).
 * Recebe os PIDs dos filhos e os pares de file descriptors dos pipes para comunicação.
 * 
 * @param gohanPid 
 * @param gotenPid 
 * @param fdGohan 
 * @param fdGoten 
 */
void pai(pid_t gohanPid, pid_t gotenPid, int fdGohan[2], int fdGoten[2]) {
    // No lado do pai fechamos as extremidades de leitura dos pipes (o pai só escreve).
    // fd[0] é a extremidade de leitura, fd[1] a de escrita.
    close(fdGohan[0]);
    close(fdGoten[0]);

    // Mensagem inicial do pai com seus PIDs e dos filhos.
    // std::format gera a string, e std::cout imprime com std::endl (para quebrar a linha).
    std::cout << std::format("[Goku]: - Oi, eu sou o Goku ({}). Meus filhos são Gohan ({}) e Goten ({})",
                             getpid(), gohanPid, gotenPid) << std::endl;

    // Variáveis usadas no loop de interação com o usuário (pai lê do stdin).
    std::string msg;
    std::string destino;

    // Loop principal: o pai pergunta qual filho deve receber a mensagem, lê a mensagem
    // e escreve no pipe apropriado.
    while (true) {
        std::cout << "[Goku] Escolha o filho (gohan/goten): " << std::endl;
        std::getline(std::cin, destino); // lê o nome do filho de destino
        
        // Se o usuário digitou "fim" o pai sai do loop. Ele ainda fecha os descritores
        // de escrita depois (fora do loop), sinalizando EOF para os filhos.
        if (destino == "fim")
            break;
            
        if (destino != "gohan" && destino != "goten") { // se não for para nenhum dos filhos, repita
            std::cout << "[Goku]: Filho inválido!" << std::endl;
            continue;
        }
        
        std::cout << "[Goku] Digite a mensagem: ";
        std::getline(std::cin, msg); // lê a mensagem

        // Dependendo do destino, escreve a mensagem no pipe do filho escolhido.
        // write(fd, buf, size) escreve bytes diretamente no pipe.
        if (destino == "gohan") {
            // +1 para enviar também o '\0' terminador de C — isso permite que o leitor
            // transforme direto em std::string terminada em NUL, se ele quiser.
            write(fdGohan[1], msg.c_str(), msg.size() + 1);
        } else if (destino == "goten") {
            write(fdGoten[1], msg.c_str(), msg.size() + 1);
        }
    }

    // Fecha os descritores de escrita: isso causa EOF nas leituras do lado dos filhos.
    close(fdGohan[1]);
    close(fdGoten[1]);

    // As chamadas wait(nullptr) foram comentadas.
    // Se estiverem presentes, o pai esperaria os filhos terminarem (bloqueando).
    // Como no main foi feito signal(SIGCHLD, SIG_IGN), os filhos são automaticamente reaped
    // em muitas implementações, por isso o autor optou por não fazer wait().
    // wait(nullptr);
    // wait(nullptr);
}

// Função executada por cada filho (Gohan ou Goten).
// Recebe o nome do filho e o par de file descriptors do pipe (o mesmo que o pai usou).
void filho(const std::string &nome, int fd[2]) {
    // No lado do filho fechamos a extremidade de escrita (o filho só lê).
    close(fd[1]);

    // Mensagem de apresentação do filho, com seu PID.
    std::cout << std::format("[{}]: - Olá, meu nome é {} ({})", nome, nome, getpid()) << std::endl;

    char buffer[256]; // buffer fixo para leitura das mensagens do pipe.
    while (true) {
        // read(fd, buf, count) lê bytes do pipe; retorna número de bytes lidos,
        // 0 em EOF (quando o escritor fechou o lado de escrita), ou -1 em erro.
        const ssize_t n = read(fd[0], buffer, sizeof(buffer) - 1);
        if (n <= 0) break;    // n == 0 -> EOF (pai fechou escrevendo); n < 0 -> erro

        // garante terminação NUL para construir std::string com segurança
        buffer[n] = '\0';
        std::string msg(buffer);

        // Exibe a mensagem recebida.
        std::cout << std::format("[{} recebeu]: {}", nome, msg) << std::endl;

        // Se a mensagem for "fim", o filho se despede e sai do loop (termina).
        if (msg == "fim") {
            std::cout << std::format("[{}]: - Tchau, pai!", nome) << std::endl;
            break;
        }
    }

    // Fecha a extremidade de leitura antes de terminar a função/processo.
    close(fd[0]);
}

int main() {
    // Ignora SIGCHLD (sinal enviado ao pai quando um filho termina).
    // Em muitas implementações POSIX, definir SIGCHLD para SIG_IGN faz com que
    // processos filhos terminados sejam automaticamente "reaped" pelo kernel
    // (não ficam como "zumbis") — por isso o pai não precisa chamar wait().
    // Observação: comportamento exato pode variar; sigaction(SA_NOCLDWAIT) é outra opção.
    signal(SIGCHLD, SIG_IGN);

    int fdGohan[2];
    int fdGoten[2]; //Cria o vetor para leitura e escrita

    // Criação dos pipes. pipe(fd) preenche fd[0] (leitura) e fd[1] (escrita).
    if (pipe(fdGohan) < 0 || pipe(fdGoten) < 0) {
        std::cerr << "Erro ao criar pipes" << std::endl;
        return EXIT_FAILURE;
    }

    // Cria o primeiro filho (Gohan).
    const pid_t pidGohan = fork();
    if (pidGohan < 0) return EXIT_FAILURE; // erro no fork
    if (pidGohan == 0) {                   // código executado pelo filho Gohan
        filho("Gohan", fdGohan);
        return 0;                          // filho termina aqui
    }

    // Cria o segundo filho (Goten).
    const pid_t pidGoten = fork();
    if (pidGoten < 0) return EXIT_FAILURE; // erro no fork
    if (pidGoten == 0) {                   // código executado pelo filho Goten
        filho("Goten", fdGoten);
        return 0;                          // filho termina aqui
    }

    // Código executado apenas pelo processo pai.
    pai(pidGohan, pidGoten, fdGohan, fdGoten);

    return 0;
}
