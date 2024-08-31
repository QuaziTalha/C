#include <iostream>
#include <conio.h> // For _getch() on Windows
#include <termios.h> // For termios functions on Linux
#include <unistd.h>  // For read() on Linux
#include <windows.h>  // For SetConsoleMode() and GetStdHandle() on Windows

#ifdef _WIN32 // Define platform-specific functions

// Windows functions
int _getch() {
  return _getch();
}

void enableRawMode() {
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  if (hStdin == INVALID_HANDLE_VALUE) {
    std::cerr << "Failed to get standard input handle." << std::endl;
    return;
  }

  DWORD dwMode;
  if (!GetConsoleMode(hStdin, &dwMode)) {
    std::cerr << "Failed to get console mode." << std::endl;
    return;
  }

  dwMode &= ~ENABLE_ECHO_INPUT; // Turn off echoing
  dwMode |= ENABLE_PROCESSED_INPUT; // Enable processing of special keys (e.g., Ctrl+C)
  if (!SetConsoleMode(hStdin, dwMode)) {
    std::cerr << "Failed to set console mode." << std::endl;
    return;
  }
}

void disableRawMode() {
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  if (hStdin == INVALID_HANDLE_VALUE) {
    std::cerr << "Failed to get standard input handle." << std::endl;
    return;
  }

  if (!SetConsoleMode(hStdin, ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT)) {
    std::cerr << "Failed to restore console mode." << std::endl;
    return;
  }
}

#else // Define platform-specific functions for Linux

// Linux functions
int _getch() {
  char c;
  read(STDIN_FILENO, &c, 1);
  return c;
}

void enableRawMode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  atexit(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw));

  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0)
    perror("tcsetattr");
}

void disableRawMode() {
  // No need to disable raw mode explicitly on Linux,
  // as exit() will automatically restore terminal settings.
}

#endif

int main() {
  enableRawMode();

  char c;
  while ((c = _getch()) != 'q') {
    if (iscntrl(c)) {
      std::cout << static_cast<int>(c) << std::endl;
    } else {
      std::cout << static_cast<int>(c) << " ('" << c << "')" << std::endl;
    }
  }

  disableRawMode();
  return 0;
}
