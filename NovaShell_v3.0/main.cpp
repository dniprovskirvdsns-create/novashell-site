#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <map>
#include <conio.h>     // Для _kbhit(), _getch()
#include <windows.h>
#include <winsvc.h>    // Для служб Windows (SCM)
#include <tlhelp32.h>
#include <psapi.h>
#include <iphlpapi.h>  // Для сетевых соединений

#pragma comment(lib, "iphlpapi.lib")

// Глобальные переменные системы
std::ofstream g_logFile;
std::time_t g_startTime = std::time(nullptr);
std::string g_currentLang = "ru";
std::map<std::string, std::map<std::string, std::string>> g_langDict;

// Инициализация словарей локализации v5.0
void initLanguageDictionary() {
    // --- Русский (ru) ---
    g_langDict["ru"]["boot_check"] = "[*] Загрузка модулей ядра v5.0...";
    g_langDict["ru"]["boot_api"] = "[*] Инициализация Win32 SCM и IP Helper API...";
    g_langDict["ru"]["boot_proc"] = "[*] Подключение диспетчера процессов и приоритетов...";
    g_langDict["ru"]["boot_config"] = "[*] Чтение файла конфигурации config.ini...";
    g_langDict["ru"]["boot_log"] = "[*] Активация системы логирования...";
    g_langDict["ru"]["boot_net"] = "[*] Инициализация сетевого монитора...";
    g_langDict["ru"]["boot_admin"] = "[*] Проверка привилегий администратора...";
    g_langDict["ru"]["boot_ready"] = "[+] Система NovaShell Enterprise v5.0 полностью готова!";
    g_langDict["ru"]["boot_launch"] = "Открытие рабочего окружения";
    g_langDict["ru"]["lang_changed"] = "[+] Язык интерфейса изменен на: ";
    g_langDict["ru"]["unknown_cmd"] = "[-] Неизвестная команда: '";
    g_langDict["ru"]["type_help"] = "'. Введите 'help' для справки.";
    
    g_langDict["ru"]["help_text"] = 
        "\n===================================================================\n"
        "         NOVASHELL ENTERPRISE v5.0 - СПРАВОЧНИК КОМАНД             \n"
        "==================================================================-\n"
        " [Система / sys]\n"
        "  sys info             - Системная статистика (RAM, CPU, Uptime)\n"
        "  sys uptime           - Время работы текущей сессии\n"
        "  sys whoami           - Имя текущего пользователя ОС\n"
        "  sys env              - Просмотр переменных окружения Windows\n"
        "  sys save [файл]      - Экспорт отчета в текстовый файл\n"
        "  sys history          - История введенных команд\n"
        "  sys color <цвет>     - Смена цвета интерфейса (green, cyan, pink, red...)\n"
        "  sys cls              - Очистка экрана\n"
        "  sys repair           - Восстановить/создать стандартные файлы консоли\n"
        "  sys openpath         - Открыть текущую папку консоли в проводнике\n"
        "  sys files            - Показать все файлы и папки проекта\n"
        "  sys clearlog         - Очистить файл логов novashell.log\n"
        "  sys reboot           - Полностью перезагрузить консоль NovaShell\n"
        "\n [Процессы / proc]\n"
        "  proc list [sort]     - Список процессов (опции: sort mem, sort pid, sort name)\n"
        "  proc monitor         - Живой монитор процессов (выход по ESC)\n"
        "  proc find <имя>      - Поиск процесса и его потребления RAM\n"
        "  proc kill <PID>      - Завершение процесса по ID\n"
        "  proc killall <имя>   - Массовое завершение процессов по имени\n"
        "  proc priority <PID> <idle|normal|high|realtime> - Изменить приоритет процесса\n"
        "\n [Службы Windows / srv]\n"
        "  srv list             - Список системных служб и их статусов\n"
        "  srv start <имя>      - Запуск службы Windows\n"
        "  srv stop <имя>       - Остановка службы Windows\n"
        "\n [Сеть / net]\n"
        "  net connections      - Просмотр активных TCP соединений в системе\n"
        "\n [Конфигурация / conf]\n"
        "  conf list            - Содержимое config.ini\n"
        "  conf add <путь>      - Добавить программу в автозапуск\n"
        "\n [Общее]\n"
        "  lang <ru|uk|en>      - Смена языка интерфейса\n"
        "  start <путь>         - Запуск приложения или оснастки (.exe, .cpl, .msc)\n"
        "  help                 - Показать эту справку\n"
        "  exit                 - Выход из системы\n"
        "==================================================================-\n\n";

    // --- Українська (uk) ---
    g_langDict["uk"]["boot_check"] = "[*] Завантаження модулів ядра v5.0...";
    g_langDict["uk"]["boot_api"] = "[*] Ініціалізація Win32 SCM та IP Helper API...";
    g_langDict["uk"]["boot_proc"] = "[*] Підключення диспетчера процесів та пріоритетів...";
    g_langDict["uk"]["boot_config"] = "[*] Читання файлу конфігурації config.ini...";
    g_langDict["uk"]["boot_log"] = "[*] Активація системи логування...";
    g_langDict["uk"]["boot_net"] = "[*] Ініціалізація мережевого монітора...";
    g_langDict["uk"]["boot_admin"] = "[*] Перевірка привілеїв адміністратора...";
    g_langDict["uk"]["boot_ready"] = "[+] Система NovaShell Enterprise v5.0 повністю готова!";
    g_langDict["uk"]["boot_launch"] = "Відкриття робочого середовища";
    g_langDict["uk"]["lang_changed"] = "[+] Мову інтерфейсу змінено на: ";
    g_langDict["uk"]["unknown_cmd"] = "[-] Невідома команда: '";
    g_langDict["uk"]["type_help"] = "'. Введіть 'help' для довідки.";

    g_langDict["uk"]["help_text"] = 
        "\n==================================================================-\n"
        "         NOVASHELL ENTERPRISE v5.0 - ДОВІДКА КОМАНД               \n"
        "==================================================================-\n"
        " [Система / sys]\n"
        "  sys info             - Системна статистика (RAM, CPU, Uptime)\n"
        "  sys uptime           - Час роботи поточної сесії\n"
        "  sys whoami           - Ім'я поточного користувача ОС\n"
        "  sys env              - Перегляд змінних середовища Windows\n"
        "  sys save [файл]      - Експорт звіту у текстовий файл\n"
        "  sys history          - Історія введених команд\n"
        "  sys color <колір>    - Зміна кольору інтерфейсу (green, cyan, pink, red...)\n"
        "  sys cls              - Очищення екрана\n"
        "  sys repair           - Відновити/створити стандартні файли консоли\n"
        "  sys openpath         - Відкрити поточну папку консоли в провіднику\n"
        "  sys files            - Показати всі файли та папки проєкту\n"
        "  sys clearlog         - Очистити файл логів novashell.log\n"
        "  sys reboot           - Повністю перезавантажити консоль NovaShell\n"
        "\n [Процеси / proc]\n"
        "  proc list [sort]     - Список процесів (опції: sort mem, sort pid, sort name)\n"
        "  proc monitor         - Живий монітор процесів (вихід по ESC)\n"
        "  proc find <ім'я>     - Пошук процесу та його RAM\n"
        "  proc kill <PID>      - Завершення процесу за ID\n"
        "  proc killall <ім'я>  - Масове завершення процесів за ім'ям\n"
        "  proc priority <PID> <idle|normal|high|realtime> - Змінити пріоритет процесу\n"
        "\n [Служби Windows / srv]\n"
        "  srv list             - Список системних служб та їх статусів\n"
        "  srv start <ім'я>     - Запуск служби Windows\n"
        "  srv stop <ім'я>      - Зупинка служби Windows\n"
        "\n [Мережа / net]\n"
        "  net connections      - Перегляд активних TCP з'єднань у системі\n"
        "\n [Конфігурація / conf]\n"
        "  conf list            - Вміст config.ini\n"
        "  conf add <шлях>      - Додати програму до автозапуску\n"
        "\n [Загальне]\n"
        "  lang <ru|uk|en>      - Зміна мови інтерфейсу\n"
        "  start <шлях>         - Запуск додатка або оснастки (.exe, .cpl, .msc)\n"
        "  help                 - Показати цю довідку\n"
        "  exit                 - Вихід із системи\n"
        "==================================================================-\n\n";

    // --- English (en) ---
    g_langDict["en"]["boot_check"] = "[*] Loading kernel modules v5.0...";
    g_langDict["en"]["boot_api"] = "[*] Initializing Win32 SCM and IP Helper API...";
    g_langDict["en"]["boot_proc"] = "[*] Connecting process and priority manager...";
    g_langDict["en"]["boot_config"] = "[*] Reading configuration file config.ini...";
    g_langDict["en"]["boot_log"] = "[*] Activating logging system...";
    g_langDict["en"]["boot_net"] = "[*] Initializing network monitor...";
    g_langDict["en"]["boot_admin"] = "[*] Verifying administrator privileges...";
    g_langDict["en"]["boot_ready"] = "[+] NovaShell Enterprise v5.0 fully ready!";
    g_langDict["en"]["boot_launch"] = "Opening workspace environment";
    g_langDict["en"]["lang_changed"] = "[+] Interface language changed to: ";
    g_langDict["en"]["unknown_cmd"] = "[-] Unknown command: '";
    g_langDict["en"]["type_help"] = "'. Type 'help' for reference.";

    g_langDict["en"]["help_text"] = 
        "\n==================================================================-\n"
        "       NOVASHELL ENTERPRISE v5.0 - COMMAND REFERENCE             \n"
        "==================================================================-\n"
        " [System / sys]\n"
        "  sys info             - System statistics (RAM, CPU, Uptime)\n"
        "  sys uptime           - Current session uptime\n"
        "  sys whoami           - Current OS user account name\n"
        "  sys env              - View Windows environment variables\n"
        "  sys save [file]      - Export system report to text file\n"
        "  sys history          - Command execution history\n"
        "  sys color <color>    - Change interface text color (green, cyan, pink, red...)\n"
        "  sys cls              - Clear screen\n"
        "  sys repair           - Repair or create standard console files\n"
        "  sys openpath         - Open current console directory in Explorer\n"
        "  sys files            - Show all project files and directories\n"
        "  sys clearlog         - Clear novashell.log file\n"
        "  sys reboot           - Completely reboot NovaShell console\n"
        "\n [Processes / proc]\n"
        "  proc list [sort]     - Process list (options: sort mem, sort pid, sort name)\n"
        "  proc monitor         - Real-time live monitor (ESC to exit)\n"
        "  proc find <name>     - Find process and check its RAM\n"
        "  proc kill <PID>      - Terminate process by ID\n"
        "  proc killall <name>  - Terminate processes by name\n"
        "  proc priority <PID> <idle|normal|high|realtime> - Change process priority\n"
        "\n [Windows Services / srv]\n"
        "  srv list             - Windows system services status list\n"
        "  srv start <name>     - Start Windows service\n"
        "  srv stop <name>      - Stop Windows service\n"
        "\n [Network / net]\n"
        "  net connections      - View active TCP connections\n"
        "\n [Configuration / conf]\n"
        "  conf list            - Show config.ini contents\n"
        "  conf add <path>      - Add program to auto-run\n"
        "\n [General]\n"
        "  lang <ru|uk|en>      - Switch interface language\n"
        "  start <path>         - Launch application or utility (.exe, .cpl, .msc)\n"
        "  help                 - Show this help reference\n"
        "  exit                 - Exit shell\n"
        "==================================================================-\n\n";
}

std::string getText(const std::string& key) {
    if (g_langDict.count(g_currentLang) && g_langDict[g_currentLang].count(key)) {
        return g_langDict[g_currentLang][key];
    }
    if (g_langDict["ru"].count(key)) {
        return g_langDict["ru"][key];
    }
    return key;
}

struct ProcessInfo {
    DWORD pid;
    double memMB;
    std::string name;
};

void writeLog(const std::string& text, bool printToConsole = true) {
    if (printToConsole) std::cout << text;
    if (g_logFile.is_open()) {
        g_logFile << text;
        g_logFile << std::flush;
    }
}

std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    char buf[80];
    struct tm tstruct;
    localtime_s(&tstruct, &now);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf);
}

// Экран загрузки v5.0
void showBootAnimation() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    std::cout << "\n\n";
    std::cout << "      _  _                 _____ _          _ _ \n";
    std::cout << "     | \\| |_____ ___ __   / ____| |___  ___| | |\n";
    std::cout << "     | .` / _ \\ V V / '_ \\| (___ | '_  \\/ _ \\ | |\n";
    std::cout << "     |_|\\_\\___/\\_/\\_/| .__/ \\____||_| |_|\\___/_|_|\n";
    std::cout << "                     |_|                          \n";
    std::cout << "         NOVASHELL ENTERPRISE SYSTEM v5.0          \n";
    std::cout << "---------------------------------------------------\n\n";

    std::vector<std::string> steps = {
        getText("boot_check"),
        getText("boot_api"),
        getText("boot_proc"),
        getText("boot_config"),
        getText("boot_log"),
        getText("boot_net"),
        getText("boot_admin"),
        getText("boot_ready")
    };

    int totalSteps = steps.size();
    int barWidth = 30;

    for (int i = 0; i < totalSteps; ++i) {
        std::cout << "\r" << steps[i] << "                                        \n";
        float progress = static_cast<float>(i + 1) / totalSteps;
        int filledWidth = static_cast<int>(barWidth * progress);

        std::cout << " [";
        for (int b = 0; b < barWidth; ++b) {
            if (b < filledWidth) std::cout << (char)219;
            else std::cout << (char)176;
        }
        std::cout << "] " << static_cast<int>(progress * 100) << "% \r";
        Sleep(500);
    }

    std::cout << "\n\n  " << getText("boot_launch");
    for (int i = 0; i < 3; ++i) {
        std::cout << "." << std::flush;
        Sleep(200);
    }
    
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void setConsoleColor(const std::string& colorName) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string color = colorName;
    std::transform(color.begin(), color.end(), color.begin(), ::tolower);

    WORD attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; 
    std::string displayName = "Белый";

    if (color == "green") { attr = FOREGROUND_GREEN; displayName = "Зеленый"; }
    else if (color == "cyan") { attr = FOREGROUND_GREEN | FOREGROUND_BLUE; displayName = "Циан"; }
    else if (color == "red") { attr = FOREGROUND_RED; displayName = "Красный"; }
    else if (color == "pink" || color == "magenta") { attr = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; displayName = "Розовый"; }
    else if (color == "yellow") { attr = FOREGROUND_RED | FOREGROUND_GREEN; displayName = "Желтый"; }
    else if (color == "brightgreen") { attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; displayName = "Ярко-зеленый"; }
    else if (color == "brightcyan") { attr = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; displayName = "Ярко-циан"; }
    else if (color == "brightred") { attr = FOREGROUND_RED | FOREGROUND_INTENSITY; displayName = "Ярко-красный"; }
    else if (color == "white") { attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; displayName = "Белый"; }
    
    SetConsoleTextAttribute(hConsole, attr);
    writeLog("[+] Цвет текста изменен: " + displayName + "\n");
}

// =========================================================================
// СИСТЕМНЫЕ ФУНКЦИИ И ФАЙЛОВЫЕ УТИЛИТЫ
// =========================================================================

void sysRepairConsoleFiles() {
    writeLog("\n[+] Запуск восстановления файлов NovaShell...\n");
    std::ifstream checkConfig("config.ini");
    if (!checkConfig.is_open()) {
        std::ofstream makeConfig("config.ini");
        if (makeConfig.is_open()) {
            makeConfig << "[AutoRun]\n; app1 = notepad.exe\n";
            makeConfig.close();
            writeLog("[+] Создан дефолтный файл конфигурации: config.ini\n");
        }
    } else {
        checkConfig.close();
        writeLog("[✓] Файл конфигурации config.ini в норме.\n");
    }

    std::ifstream checkLog("novashell.log");
    if (!checkLog.is_open()) {
        std::ofstream makeLog("novashell.log");
        if (makeLog.is_open()) {
            makeLog << "=== NovaShell Log Initialized ===\n";
            makeLog.close();
            writeLog("[+] Создан чистый файл логов: novashell.log\n");
        }
    } else {
        checkLog.close();
        writeLog("[✓] Система логирования (novashell.log) активна.\n");
    }
    writeLog("[+] Восстановление файлов консоли успешно завершено!\n\n");
}

void sysOpenConsolePath() {
    writeLog("[i] Открытие рабочей папки консоли в Проводнике Windows...\n");
    HINSTANCE result = ShellExecuteA(NULL, "open", ".", NULL, NULL, SW_SHOWNORMAL);
    if (reinterpret_cast<INT_PTR>(result) > 32) writeLog("[+] Папка успешно открыта!\n\n");
    else writeLog("[-] Ошибка при открытии папки через Проводник.\n\n");
}

void sysShowConsoleFiles() {
    writeLog("\n=== Список файлов и папок в рабочей директории NovaShell ===\n");
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("*.*", &findData);
    if (hFind == INVALID_HANDLE_VALUE) { writeLog("[-] Не удалось прочитать директорию.\n\n"); return; }

    int fileCount = 0, dirCount = 0;
    do {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::cout << " [DIR]  " << findData.cFileName << "\n";
            dirCount++;
        } else {
            std::cout << " [FILE] " << findData.cFileName << "\n";
            fileCount++;
        }
    } while (FindNextFileA(hFind, &findData) != 0);
    FindClose(hFind);

    char buf[128];
    std::snprintf(buf, sizeof(buf), "\n Всего папок: %d | Всего файлов: %d\n============================================================\n\n", dirCount, fileCount);
    writeLog(buf);
}

// Маленькое обновление 3: Очистка логов (sys clearlog)
void sysClearLogFile() {
    if (g_logFile.is_open()) g_logFile.close();
    std::ofstream clearLog("novashell.log", std::ios::trunc);
    if (clearLog.is_open()) {
        clearLog << "=== NovaShell Log Cleared: " << getCurrentTime() << " ===\n";
        clearLog.close();
        writeLog("[+] Файл логов novashell.log успешно очищен.\n\n");
    } else {
        writeLog("[-] Не удалось очистить файл логов.\n\n");
    }
    g_logFile.open("novashell.log", std::ios::app);
}

void sysRebootConsole() {
    writeLog("\n[!] Перезагрузка NovaShell Enterprise...\n");
    Sleep(1000);
    char szPath[MAX_PATH];
    GetModuleFileNameA(NULL, szPath, MAX_PATH);
    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi;
    if (CreateProcessA(szPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
        if (g_logFile.is_open()) g_logFile.close();
        std::exit(0);
    } else {
        writeLog("[-] Ошибка перезагрузки консоли.\n\n");
    }
}

// Маленькое обновление 1: Вывод переменных окружения (sys env)
void showEnvironmentVariables() {
    writeLog("\n=== Системные переменные окружения Windows ===\n");
    LPCH envBlock = GetEnvironmentStringsA();
    if (envBlock != NULL) {
        LPCH var = envBlock;
        while (*var != '\0') {
            std::cout << " " << var << "\n";
            var += strlen(var) + 1;
        }
        FreeEnvironmentStringsA(envBlock);
    }
    writeLog("=============================================\n\n");
}

// Крупное обновление v5.0: Изменение приоритета процесса
void setProcessPriority(DWORD pid, const std::string& level) {
    if (pid == 0) { writeLog("[-] Некорректный PID.\n"); return; }
    
    DWORD priorityClass = NORMAL_PRIORITY_CLASS;
    if (level == "idle") priorityClass = IDLE_PRIORITY_CLASS;
    else if (level == "normal") priorityClass = NORMAL_PRIORITY_CLASS;
    else if (level == "high") priorityClass = HIGH_PRIORITY_CLASS;
    else if (level == "realtime") priorityClass = REALTIME_PRIORITY_CLASS;
    else {
        writeLog("[-] Неверный уровень. Используйте: idle, normal, high, realtime\n");
        return;
    }

    HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
    if (hProc == NULL) { writeLog("[-] Не удалось открыть процесс с PID " + std::to_string(pid) + "\n"); return; }

    if (SetPriorityClass(hProc, priorityClass)) {
        writeLog("[+] Приоритет процесса PID " + std::to_string(pid) + " успешно изменен на '" + level + "'.\n");
    } else {
        writeLog("[-] Ошибка изменения приоритета (для realtime нужны права администратора).\n");
    }
    CloseHandle(hProc);
}

// =========================================================================

double getProcessMemoryMB(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == NULL) return 0.0;
    PROCESS_MEMORY_COUNTERS pmc;
    double memSizeMB = 0.0;
    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        memSizeMB = static_cast<double>(pmc.WorkingSetSize) / (1024 * 1024);
    }
    CloseHandle(hProcess);
    return memSizeMB;
}

void showNetworkConnections() {
    writeLog("\n[+] Сканирование активных сетевых соединений (TCP)...\n");
    ULONG bufferSize = 0;
    GetExtendedTcpTable(NULL, &bufferSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    std::vector<BYTE> buffer(bufferSize);
    
    PMIB_TCPTABLE_OWNER_PID tcpTable = reinterpret_cast<PMIB_TCPTABLE_OWNER_PID>(buffer.data());
    if (GetExtendedTcpTable(tcpTable, &bufferSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
        writeLog("-------------------------------------------------------------------\n Локальный IP:порт  | Удаленный IP:порт   | Статус      | PID\n-------------------------------------------------------------------\n");
        for (DWORD i = 0; i < tcpTable->dwNumEntries; ++i) {
            MIB_TCPROW_OWNER_PID row = tcpTable->table[i];
            char localIp[16], remoteIp[16];
            in_addr lAddr, rAddr;
            lAddr.s_addr = row.dwLocalAddr;
            rAddr.s_addr = row.dwRemoteAddr;
            strcpy_s(localIp, inet_ntoa(lAddr));
            strcpy_s(remoteIp, inet_ntoa(rAddr));
            
            u_short localPort = ntohs(static_cast<u_short>(row.dwLocalPort));
            u_short remotePort = ntohs(static_cast<u_short>(row.dwRemotePort));
            
            std::string state = "UNKNOWN";
            if (row.dwState == MIB_TCP_STATE_ESTAB) state = "ESTABLISHED";
            else if (row.dwState == MIB_TCP_STATE_LISTEN) state = "LISTENING";
            else if (row.dwState == MIB_TCP_STATE_TIME_WAIT) state = "TIME_WAIT";

            char line[256];
            std::snprintf(line, sizeof(line), " %-16s | %-16s | %-11s | %u\n", 
                (std::string(localIp) + ":" + std::to_string(localPort)).c_str(),
                (std::string(remoteIp) + ":" + std::to_string(remotePort)).c_str(),
                state.c_str(),
                row.dwOwningPid);
            writeLog(line);
        }
        writeLog("-------------------------------------------------------------------\n\n");
    } else {
        writeLog("[-] Ошибка получения сетевой таблицы.\n\n");
    }
}

void findProcess(const std::string& query) {
    if (query.empty()) { writeLog("[-] Использование: proc find <имя>\n"); return; }
    writeLog("\n[+] Поиск процессов: '" + query + "'...\n");
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe32; pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe32)) { CloseHandle(hSnapshot); return; }

    writeLog("-------------------------------------------------------------------\n PID      | Память (RAM) | Имя процесса\n-------------------------------------------------------------------\n");
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    int found = 0;

    do {
        std::string exeName(pe32.szExeFile);
        std::string lowerExe = exeName;
        std::transform(lowerExe.begin(), lowerExe.end(), lowerExe.begin(), ::tolower);
        if (lowerExe.find(lowerQuery) != std::string::npos) {
            double memMB = getProcessMemoryMB(pe32.th32ProcessID);
            char buf[512];
            std::snprintf(buf, sizeof(buf), " %-8u | %6.1f MB   | %s\n", pe32.th32ProcessID, memMB, exeName.c_str());
            writeLog(buf);
            found++;
        }
    } while (Process32Next(hSnapshot, &pe32));
    writeLog("-------------------------------------------------------------------\n Найдено совпадений: " + std::to_string(found) + "\n\n");
    CloseHandle(hSnapshot);
}

void exportSystemReport(const std::string& filename) {
    std::string reportFile = filename.empty() ? "novashell_report.txt" : filename;
    writeLog("[+] Создание системного отчета...\n");
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe32; pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe32)) { CloseHandle(hSnapshot); return; }

    std::ofstream outFile(reportFile, std::ios::trunc);
    if (!outFile.is_open()) { CloseHandle(hSnapshot); return; }

    outFile << "=========================================================\n";
    outFile << "          NOVASHELL ENTERPRISE SYSTEM REPORT             \n";
    outFile << "  Дата и время: " << getCurrentTime() << "\n";
    outFile << "=========================================================\n\n";

    int count = 0;
    do {
        double memMB = getProcessMemoryMB(pe32.th32ProcessID);
        char buf[512];
        std::snprintf(buf, sizeof(buf), " PID: %-6u | RAM: %6.1f MB | %s", pe32.th32ProcessID, memMB, pe32.szExeFile);
        outFile << buf << "\n";
        count++;
    } while (Process32Next(hSnapshot, &pe32));

    outFile << "=========================================================\n";
    outFile << " Всего процессов: " << count << "\n";
    outFile.close();
    CloseHandle(hSnapshot);
    writeLog("[+] Отчет успешно сохранен в файл: " + reportFile + "\n\n");
}

void showWhoAmI() {
    char username[256];
    DWORD len = sizeof(username);
    if (GetUserNameA(username, &len)) writeLog("[+] Пользователь ОС: " + std::string(username) + "\n\n");
    else writeLog("[-] Не удалось получить имя пользователя.\n\n");
}

void showUptime() {
    std::time_t now = std::time(nullptr);
    long sec = static_cast<long>(std::difftime(now, g_startTime));
    char buf[128];
    std::snprintf(buf, sizeof(buf), "[+] Uptime сессии: %02ld:%02ld:%02ld\n\n", sec / 3600, (sec % 3600) / 60, sec % 60);
    writeLog(buf);
}

void showSystemInfo() {
    std::time_t now = std::time(nullptr);
    long sec = static_cast<long>(std::difftime(now, g_startTime));
    MEMORYSTATUSEX mem; mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);

    int proc = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe; pe.dwSize = sizeof(pe);
        if (Process32First(hSnap, &pe)) do { proc++; } while (Process32Next(hSnap, &pe));
        CloseHandle(hSnap);
    }

    char buf[512];
    writeLog("\n=== Системная статистика NovaShell v5.0 (" + g_currentLang + ") ===\n");
    std::snprintf(buf, sizeof(buf), " Uptime:              %02ld:%02ld:%02ld\n", sec / 3600, (sec % 3600) / 60, sec % 60); writeLog(buf);
    std::snprintf(buf, sizeof(buf), " Активных процессов:   %d\n", proc); writeLog(buf);
    std::snprintf(buf, sizeof(buf), " Загрузка памяти RAM:  %lu%%\n", mem.dwMemoryLoad); writeLog(buf);
    std::snprintf(buf, sizeof(buf), " Всего физической RAM: %.2f GB\n", static_cast<double>(mem.ullTotalPhys) / (1024*1024*1024)); writeLog(buf);
    std::snprintf(buf, sizeof(buf), " Доступно физической:  %.2f GB\n", static_cast<double>(mem.ullAvailPhys) / (1024*1024*1024)); writeLog(buf);
    writeLog("===========================================\n\n");
}

void listProcesses(const std::string& sortBy = "") {
    writeLog("\n[+] Получение списка активных процессов...\n");
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe; pe.dwSize = sizeof(pe);
    if (!Process32First(hSnap, &pe)) { CloseHandle(hSnap); return; }

    std::vector<ProcessInfo> list;
    do {
        list.push_back({pe.th32ProcessID, getProcessMemoryMB(pe.th32ProcessID), pe.szExeFile});
    } while (Process32Next(hSnap, &pe));
    CloseHandle(hSnap);

    if (sortBy == "mem") std::sort(list.begin(), list.end(), [](auto& a, auto& b) { return a.memMB > b.memMB; });
    else if (sortBy == "pid") std::sort(list.begin(), list.end(), [](auto& a, auto& b) { return a.pid < b.pid; });
    else if (sortBy == "name") std::sort(list.begin(), list.end(), [](auto& a, auto& b) { return a.name < b.name; });

    writeLog("-------------------------------------------------------------------\n PID      | Память (RAM) | Имя процесса\n-------------------------------------------------------------------\n");
    for (const auto& p : list) {
        char buf[512];
        std::snprintf(buf, sizeof(buf), " %-8u | %6.1f MB   | %s\n", p.pid, p.memMB, p.name.c_str());
        writeLog(buf);
    }
    writeLog("-------------------------------------------------------------------\nВсего процессов: " + std::to_string(list.size()) + "\n\n");
}

void runInteractiveMonitor() {
    writeLog("\n[i] Интерактивный монитор запущен. Нажмите ESC для выхода...\n\n");
    Sleep(1000);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            while (_kbhit()) _getch();
            writeLog("\n[i] Выход из мониторинга.\n\n");
            break;
        }
        COORD coord = { 0, 0 };
        SetConsoleCursorPosition(hConsole, coord);

        std::cout << "==================================================================-\n";
        std::cout << "          NOVASHELL LIVE MONITOR (Обновление 1 сек)                \n";
        std::cout << "          Для выхода нажмите [ESC]                                 \n";
        std::cout << "==================================================================-\n";
        std::cout << " PID      | Память (RAM) | Имя процесса\n";
        std::cout << "-------------------------------------------------------------------\n";

        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe; pe.dwSize = sizeof(pe);
            if (Process32First(hSnap, &pe)) {
                std::vector<ProcessInfo> list;
                do {
                    list.push_back({pe.th32ProcessID, getProcessMemoryMB(pe.th32ProcessID), pe.szExeFile});
                } while (Process32Next(hSnap, &pe));
                CloseHandle(hSnap);

                std::sort(list.begin(), list.end(), [](auto& a, auto& b) { return a.memMB > b.memMB; });
                int count = 0;
                for (const auto& p : list) {
                    if (count < 20) {
                        char buf[512];
                        std::snprintf(buf, sizeof(buf), " %-8u | %6.1f MB   | %-40s\n", p.pid, p.memMB, p.name.c_str());
                        std::cout << buf;
                        count++;
                    }
                }
            }
        }
        Sleep(1000);
    }
}

void killProcess(DWORD pid) {
    if (pid == 0) return;
    HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!hProc) { writeLog("[-] Ошибка открытия процесса.\n"); return; }
    if (TerminateProcess(hProc, 0)) writeLog("[+] Процесс PID " + std::to_string(pid) + " завершен.\n");
    else writeLog("[-] Ошибка завершения.\n");
    CloseHandle(hProc);
}

void killAllProcesses(const std::string& name) {
    if (name.empty()) return;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return;
    PROCESSENTRY32 pe; pe.dwSize = sizeof(pe);
    if (!Process32First(hSnap, &pe)) { CloseHandle(hSnap); return; }

    std::string lowerT = name;
    std::transform(lowerT.begin(), lowerT.end(), lowerT.begin(), ::tolower);
    int count = 0;

    do {
        std::string exe(pe.szExeFile);
        std::string lowerE = exe;
        std::transform(lowerE.begin(), lowerE.end(), lowerE.begin(), ::tolower);
        if (lowerE == lowerT || lowerE == lowerT + ".exe") {
            if (exe == "novashell.exe") continue;
            HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
            if (hProc) {
                if (TerminateProcess(hProc, 0)) count++;
                CloseHandle(hProc);
            }
        }
    } while (Process32Next(hSnap, &pe));
    CloseHandle(hSnap);
    writeLog("[+] Завершено процессов '" + name + "': " + std::to_string(count) + "\n\n");
}

void startProcess(const std::wstring& path) {
    std::wstring adj = path;
    std::wstring low = path;
    std::transform(low.begin(), low.end(), low.begin(), ::towlower);
    if (low.length() >= 4 && low.substr(low.length() - 4) == L".cpl") adj = L"control.exe " + path;
    else if (low.length() >= 4 && low.substr(low.length() - 4) == L".msc") adj = L"mmc.exe " + path;

    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION pi;
    std::vector<wchar_t> cmd(adj.begin(), adj.end());
    cmd.push_back(0);

    if (CreateProcessW(NULL, cmd.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        writeLog("[+] Процесс запущен! PID: " + std::to_string(pi.dwProcessId) + "\n");
        CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    } else {
        writeLog("[-] Ошибка запуска процесса.\n");
    }
}

void handleServiceCommand(std::stringstream& ss) {
    std::string sub; ss >> sub;
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
    if (!hSCM) { writeLog("[-] Ошибка SCM (нужны права администратора).\n"); return; }

    if (sub == "list") {
        DWORD needed = 0, returned = 0, resume = 0;
        EnumServicesStatusExA(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &needed, &returned, &resume, NULL);
        std::vector<BYTE> buf(needed);
        EnumServicesStatusExA(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, buf.data(), needed, &needed, &returned, &resume, NULL);
        LPENUM_SERVICE_STATUS_PROCESSA srv = (LPENUM_SERVICE_STATUS_PROCESSA)buf.data();

        writeLog("-------------------------------------------------------------------\n Статус      | Имя службы         | Отображаемое имя\n-------------------------------------------------------------------\n");
        for (DWORD i = 0; i < returned; ++i) {
            std::string st = (srv[i].ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING) ? "Работает  " : "Остановлена";
            char line[512];
            std::snprintf(line, sizeof(line), " %-11s | %-18s | %s\n", st.c_str(), srv[i].lpServiceName, srv[i].lpDisplayName);
            writeLog(line);
        }
        writeLog("-------------------------------------------------------------------\n\n");
    } else if (sub == "start" || sub == "stop") {
        std::string name; ss >> name;
        SC_HANDLE hSrv = OpenServiceA(hSCM, name.c_str(), (sub == "start" ? SERVICE_START : SERVICE_STOP) | SERVICE_QUERY_STATUS);
        if (hSrv) {
            if (sub == "start") StartServiceA(hSrv, 0, NULL);
            else { SERVICE_STATUS st; ControlService(hSrv, SERVICE_CONTROL_STOP, &st); }
            writeLog("[+] Служба '" + name + "' " + (sub == "start" ? "запущена" : "остановлена") + ".\n");
            CloseServiceHandle(hSrv);
        } else writeLog("[-] Не удалось открыть службу.\n");
    }
    CloseHandle(hSCM);
}

void handleConfigCommand(std::stringstream& ss) {
    std::string sub; ss >> sub;
    if (sub == "list") {
        std::ifstream file("config.ini");
        if (!file.is_open()) { writeLog("[-] Файл config.ini не найден.\n"); return; }
        writeLog("\n=== config.ini ===\n");
        std::string l; while (std::getline(file, l)) writeLog(l + "\n");
        writeLog("==================\n\n");
        file.close();
    } else if (sub == "add") {
        std::string path; std::getline(ss, path);
        if (!path.empty() && path[0] == ' ') path.erase(0, 1);
        std::ofstream file("config.ini", std::ios::app);
        if (file.is_open()) { file << path << "\n"; file.close(); writeLog("[+] Добавлено в config.ini\n"); }
    }
}

void handleLanguageCommand(std::stringstream& ss) {
    std::string lang; ss >> lang;
    std::transform(lang.begin(), lang.end(), lang.begin(), ::tolower);
    if (lang == "ru" || lang == "uk" || lang == "en") {
        g_currentLang = lang;
        writeLog(getText("lang_changed") + g_currentLang + "\n");
    } else {
        writeLog("[-] Использование: lang <ru | uk | en>\n");
    }
}

// Маленькое обновление 2: Анимированный выход из консоли
void handleExitSequence() {
    writeLog("\n[i] Выключение NovaShell Enterprise");
    for (int i = 0; i < 3; ++i) {
        std::cout << "." << std::flush;
        Sleep(250);
    }
    writeLog("\n[✓] Сеанс завершен успешно. До свидания!\n");
}

std::string readLineWithHistory(const std::vector<std::string>& history) {
    std::string line = "";
    int idx = history.size();
    std::cout << "NovaShell> " << std::flush;

    while (true) {
        int ch = _getch();
        if (ch == 13) { std::cout << "\n"; break; }
        else if (ch == 8) {
            if (!line.empty()) { line.pop_back(); std::cout << "\b \b" << std::flush; }
        }
        else if (ch == 0 || ch == 224) {
            int scan = _getch();
            if (scan == 72 && !history.empty() && idx > 0) {
                idx--;
                for (size_t i = 0; i < line.length(); ++i) std::cout << "\b \b";
                line = history[idx]; std::cout << line << std::flush;
            } else if (scan == 80) {
                for (size_t i = 0; i < line.length(); ++i) std::cout << "\b \b";
                if (!history.empty() && idx < (int)history.size() - 1) { idx++; line = history[idx]; }
                else { idx = history.size(); line = ""; }
                std::cout << line << std::flush;
            }
        } else if (ch >= 32 && ch <= 126) {
            line += (char)ch; std::cout << (char)ch << std::flush;
        }
    }
    return line;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    initLanguageDictionary();

    g_logFile.open("novashell.log", std::ios::app);
    showBootAnimation();

    std::vector<std::string> history;
    std::string input;

    while (true) {
        input = readLineWithHistory(history);
        if (input.empty()) continue;
        if (history.empty() || history.back() != input) history.push_back(input);

        std::stringstream ss(input);
        std::string cmd; ss >> cmd;

        if (cmd == "exit") {
            handleExitSequence();
            break;
        }
        else if (cmd == "help") writeLog(getText("help_text"));
        else if (cmd == "lang") handleLanguageCommand(ss);
        else if (cmd == "start") {
            std::string p; std::getline(ss, p);
            if (!p.empty() && p[0] == ' ') p.erase(0, 1);
            if (!p.empty()) startProcess(std::wstring(p.begin(), p.end()));
        }
        // Модуль SYS (Система)
        else if (cmd == "sys") {
            std::string sub; ss >> sub;
            if (sub == "info") showSystemInfo();
            else if (sub == "uptime") showUptime();
            else if (sub == "whoami") showWhoAmI();
            else if (sub == "env") showEnvironmentVariables();
            else if (sub == "save") { std::string f; ss >> f; exportSystemReport(f); }
            else if (sub == "repair") sysRepairConsoleFiles();
            else if (sub == "openpath") sysOpenConsolePath();
            else if (sub == "files") sysShowConsoleFiles();
            else if (sub == "clearlog") sysClearLogFile();
            else if (sub == "reboot") sysRebootConsole();
            else if (sub == "history") {
                writeLog("\n=== История команд ===\n");
                for (size_t i = 0; i < history.size(); ++i) {
                    char buf[512]; std::snprintf(buf, sizeof(buf), " [%2zu] %s\n", i + 1, history[i].c_str());
                    writeLog(buf);
                }
                writeLog("======================\n\n");
            }
            else if (sub == "color") { std::string c; ss >> c; setConsoleColor(c); }
            else if (sub == "cls") system("cls");
        }
        // Модуль PROC (Процессы)
        else if (cmd == "proc") {
            std::string sub; ss >> sub;
            if (sub == "list") {
                std::string arg1, arg2; ss >> arg1;
                if (arg1 == "sort") { ss >> arg2; listProcesses(arg2); }
                else listProcesses("");
            }
            else if (sub == "monitor") runInteractiveMonitor();
            else if (sub == "find") { std::string q; ss >> q; findProcess(q); }
            else if (sub == "kill") { DWORD pid = 0; if (ss >> pid) killProcess(pid); }
            else if (sub == "killall") { std::string t; ss >> t; killAllProcesses(t); }
            else if (sub == "priority") {
                DWORD pid = 0; std::string level;
                if (ss >> pid >> level) setProcessPriority(pid, level);
                else writeLog("[-] Использование: proc priority <PID> <idle|normal|high|realtime>\n");
            }
        }
        // Модуль SRV (Службы)
        else if (cmd == "srv") {
            handleServiceCommand(ss);
        }
        // Модуль NET (Сеть)
        else if (cmd == "net") {
            std::string sub; ss >> sub;
            if (sub == "connections") showNetworkConnections();
        }
        // Модуль CONF (Конфигурация)
        else if (cmd == "conf") {
            handleConfigCommand(ss);
        }
        // Обратная совместимость
        else if (cmd == "info") showSystemInfo();
        else if (cmd == "ps") listProcesses("");
        else if (cmd == "monitor") runInteractiveMonitor();
        else if (cmd == "cls") system("cls");
        else writeLog(getText("unknown_cmd") + cmd + getText("type_help") + "\n");
    }

    if (g_logFile.is_open()) g_logFile.close();
    return 0;
}