# 🚀 Simple Crawler in C++

A **lightweight** and **efficient** web crawler written in C++ that:

- Downloads web pages
- Extracts links and keywords (excluding stop words)
- Recursively crawls pages up to a user-defined depth  
- Saves crawled pages and keyword indexes  
- Provides clear console output for crawl progress and errors  

---

## ✨ Features

- Downloads web pages using [`wget`](https://eternallybored.org/misc/wget/) (Windows environment)  
- Extracts and normalizes hyperlinks from downloaded HTML pages  
- Extracts meaningful keywords excluding common stop words  
- Recursively crawls discovered links up to a **maximum depth**  
- Prevents revisiting the same URLs to avoid cycles  
- Saves downloaded pages and keyword indexes to disk for later analysis  
- Informative and clean console output showing crawl progress and error messages  

---

## 🛠 Requirements

- **Windows OS** (due to Windows-specific directory and system calls)  
- `wget` installed at `C:\ProgramData\chocolatey\bin\wget.exe`  
- C++ compiler supporting C++11 or later (e.g., `g++`, MSVC)  
- Standard C++ libraries  

---

## 🚀 Getting Started

### Build

Compile the project with your preferred C++ compiler. For example, using `g++`:

```bash
g++ main.cpp crawler.cpp stringutils.cpp stopwords.cpp -o crawler.exe


