# 🛠️ Development Environment Setup Template

Complete checklist and setup guide for DevTeam6 development environment.

---

## 📋 Overview

This document serves as a **comprehensive template** for setting up a complete development environment for DevTeam6. Use the checkboxes to track your setup progress.

**Quick Setup**: Use our automated scripts in the `scripts/` directory:
- Linux/macOS: `./scripts/setup-linux.sh` or `./scripts/setup-macos.sh`
- Windows: `.\scripts\setup-windows.ps1`
- Verification: `./scripts/verify-environment.sh` or `.\scripts\verify-environment.ps1`

---

## 🐍 Python Ecosystem

### Core Python
- [ ] **python3** - Python 3.10+ (Recommended: 3.12.11)
  ```bash
  python3 --version  # Should show 3.10+
  ```

- [ ] **pip** - Python package installer
  ```bash
  pip --version  # Latest: 25.1.1+
  ```

### Python Version Management
- [ ] **pyenv** - Python version manager
  ```bash
  curl https://pyenv.run | bash
  pyenv --version
  ```
  
- [ ] **pyenv versions** - Install multiple Python versions
  ```bash
  pyenv install 3.10.18
  pyenv install 3.12.11
  pyenv global 3.12.11
  ```

### Python Package Managers
- [ ] **pipx** - Install Python CLI apps in isolated environments
  ```bash
  python3 -m pip install --user pipx
  python3 -m pipx ensurepath
  pipx --version  # 1.4.3+
  ```

- [ ] **poetry** - Dependency management and packaging
  ```bash
  pipx install poetry
  poetry --version  # 2.1.3+
  ```

- [ ] **uv** - Ultra-fast Python package installer
  ```bash
  pipx install uv
  uv --version  # 0.7.13+
  ```

### Python Development Tools
- [ ] **black** - Code formatter
  ```bash
  pipx install black
  black --version  # 25.1.0+
  ```

- [ ] **mypy** - Static type checker
  ```bash
  pipx install mypy
  mypy --version  # 1.16.1+
  ```

- [ ] **pytest** - Testing framework
  ```bash
  pipx install pytest
  pytest --version  # 8.4.0+
  ```

- [ ] **ruff** - Fast Python linter
  ```bash
  pipx install ruff
  ruff --version  # 0.12.0+
  ```

- [ ] **pylint** - Additional linting
  ```bash
  pipx install pylint
  ```

- [ ] **isort** - Import sorting
  ```bash
  pipx install isort
  ```

---

## 📦 Node.js Ecosystem

### Core Node.js
- [ ] **nvm** - Node Version Manager
  ```bash
  # Linux/macOS
  curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.0/install.sh | bash
  
  # Windows
  # Download from: https://github.com/coreybutler/nvm-windows/releases
  ```

- [ ] **node** - Node.js runtime (Multiple versions)
  ```bash
  nvm install 18.20.8
  nvm install 20.19.2
  nvm install 22.16.0
  nvm use 22.16.0  # Set as active
  node --version
  ```

### Node.js Package Managers
- [ ] **npm** - Default Node package manager
  ```bash
  npm --version  # 11.4.2+
  ```

- [ ] **yarn** - Alternative package manager
  ```bash
  npm install -g yarn
  yarn --version  # 1.22.22+
  ```

- [ ] **pnpm** - Fast, disk space efficient package manager
  ```bash
  npm install -g pnpm
  pnpm --version  # 10.12.1+
  ```

### Node.js Development Tools
- [ ] **eslint** - JavaScript/TypeScript linter
  ```bash
  npm install -g eslint
  eslint --version  # 9.29.0+
  ```

- [ ] **prettier** - Code formatter
  ```bash
  npm install -g prettier
  prettier --version  # 3.5.3+
  ```

- [ ] **typescript** - TypeScript compiler
  ```bash
  npm install -g typescript
  tsc --version
  ```

- [ ] **ts-node** - TypeScript execution engine
  ```bash
  npm install -g ts-node
  ```

- [ ] **nodemon** - Auto-restart development tool
  ```bash
  npm install -g nodemon
  ```

### Browser Testing
- [ ] **chromedriver** - Chrome WebDriver for Selenium
  ```bash
  npm install -g chromedriver
  chromedriver --version  # 137.0+
  ```

---

## ☕ Java Ecosystem

- [ ] **java** - Java Development Kit (JDK 21+)
  ```bash
  # Ubuntu/Debian
  sudo apt install openjdk-21-jdk
  
  # macOS
  brew install openjdk@21
  
  java --version  # 21.0.7+
  ```

- [ ] **maven** - Build automation tool
  ```bash
  # Ubuntu/Debian
  sudo apt install maven
  
  # macOS
  brew install maven
  
  mvn --version  # 3.9.10+
  ```

- [ ] **gradle** - Build automation tool
  ```bash
  # Ubuntu/Debian
  sudo apt install gradle
  
  # macOS
  brew install gradle
  
  gradle --version  # 8.8+
  ```

- [ ] **JAVA_HOME** - Environment variable
  ```bash
  export JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64
  ```

---

## 🔷 .NET Ecosystem

- [ ] **.NET SDK** - .NET development kit (for dotnet8-api template)
  ```bash
  # Ubuntu/Debian
  wget https://dot.net/v1/dotnet-install.sh
  chmod +x dotnet-install.sh
  ./dotnet-install.sh --channel 8.0
  
  # macOS
  brew install --cask dotnet-sdk
  
  # Windows
  # Download from: https://dotnet.microsoft.com/download
  
  dotnet --version  # 8.0+
  ```

- [ ] **NuGet** - .NET package manager (included with .NET SDK)
  ```bash
  dotnet nuget --version
  ```

---

## 🦀 Rust Ecosystem

- [ ] **rustc** - Rust compiler
  ```bash
  curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
  rustc --version  # 1.87.0+
  ```

- [ ] **cargo** - Rust package manager
  ```bash
  cargo --version  # 1.87.0+
  ```

- [ ] **rustfmt** - Rust code formatter
  ```bash
  rustup component add rustfmt
  ```

- [ ] **clippy** - Rust linter
  ```bash
  rustup component add clippy
  ```

---

## 🐹 Go Ecosystem

- [ ] **go** - Go programming language
  ```bash
  # Ubuntu/Debian
  wget https://go.dev/dl/go1.24.3.linux-amd64.tar.gz
  sudo tar -C /usr/local -xzf go1.24.3.linux-amd64.tar.gz
  
  # macOS
  brew install go
  
  go version  # 1.24.3+
  ```

- [ ] **GOPATH** - Environment variable
  ```bash
  export GOPATH=$HOME/go
  export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin
  ```

- [ ] **golangci-lint** - Go linter
  ```bash
  go install github.com/golangci/golangci-lint/cmd/golangci-lint@latest
  ```

---

## 🔧 C/C++ Ecosystem

### Compilers
- [ ] **gcc** - GNU C/C++ Compiler
  ```bash
  # Ubuntu/Debian
  sudo apt install build-essential
  
  # macOS
  xcode-select --install
  
  gcc --version  # 13.3.0+
  g++ --version
  ```

- [ ] **clang** - LLVM C/C++ Compiler
  ```bash
  # Ubuntu/Debian
  sudo apt install clang
  
  # macOS (included with Xcode)
  
  clang --version  # 18.1.3+
  ```

### Build Tools
- [ ] **cmake** - Cross-platform build system
  ```bash
  # Ubuntu/Debian
  sudo apt install cmake
  
  # macOS
  brew install cmake
  
  cmake --version  # 3.28.3+
  ```

- [ ] **ninja** - Build system
  ```bash
  # Ubuntu/Debian
  sudo apt install ninja-build
  
  # macOS
  brew install ninja
  
  ninja --version  # 1.11.1+
  ```

- [ ] **make** - GNU Make
  ```bash
  make --version  # 4.3+
  ```

### Package Managers
- [ ] **conan** - C/C++ package manager
  ```bash
  pipx install conan
  conan --version  # 2.17.0+
  ```

- [ ] **vcpkg** - C/C++ package manager (Microsoft)
  ```bash
  git clone https://github.com/Microsoft/vcpkg.git
  cd vcpkg
  ./bootstrap-vcpkg.sh  # Linux/macOS
  # .\bootstrap-vcpkg.bat  # Windows
  ```

---

## 🐳 Container & Orchestration

### Docker
- [ ] **docker** - Container platform
  ```bash
  # Ubuntu/Debian
  curl -fsSL https://get.docker.com -o get-docker.sh
  sudo sh get-docker.sh
  
  # macOS
  # Download Docker Desktop from: https://www.docker.com/products/docker-desktop
  
  docker --version  # 28.2.2+
  ```

- [ ] **docker-compose** - Multi-container orchestration
  ```bash
  # Included with Docker Desktop
  # Or install separately:
  sudo curl -L "https://github.com/docker/compose/releases/latest/download/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
  sudo chmod +x /usr/local/bin/docker-compose
  
  docker-compose --version  # 2.36.2+
  ```

- [ ] **Docker user group** (Linux)
  ```bash
  sudo usermod -aG docker $USER
  # Log out and back in for changes to take effect
  ```

### Kubernetes
- [ ] **kubectl** - Kubernetes CLI
  ```bash
  # Ubuntu/Debian
  curl -LO "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl"
  sudo install -o root -g root -m 0755 kubectl /usr/local/bin/kubectl
  
  # macOS
  brew install kubectl
  
  kubectl version --client
  ```

- [ ] **helm** - Kubernetes package manager
  ```bash
  # Ubuntu/Debian
  curl https://raw.githubusercontent.com/helm/helm/main/scripts/get-helm-3 | bash
  
  # macOS
  brew install helm
  
  helm version
  ```

- [ ] **k9s** - Kubernetes TUI
  ```bash
  # Ubuntu/Debian
  brew install k9s  # Via Linuxbrew
  
  # macOS
  brew install k9s
  ```

- [ ] **minikube** - Local Kubernetes
  ```bash
  # Ubuntu/Debian
  curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
  sudo install minikube-linux-amd64 /usr/local/bin/minikube
  
  # macOS
  brew install minikube
  ```

---

## 🗄️ Databases & Data Stores

### Vector Databases
- [ ] **Qdrant** - Vector similarity search engine
  ```bash
  # Docker (Recommended)
  docker pull qdrant/qdrant
  docker run -p 6333:6333 -p 6334:6334 \
    -v $(pwd)/qdrant_storage:/qdrant/storage:z \
    qdrant/qdrant
  
  # Or install binary:
  # Download from: https://github.com/qdrant/qdrant/releases
  
  # Python client
  pip install qdrant-client
  
  # Verify
  curl http://localhost:6333/
  ```

- [ ] **ChromaDB** - Vector database (already used in local-ai)
  ```bash
  pip install chromadb
  ```

- [ ] **Milvus** - Vector database alternative
  ```bash
  docker pull milvusdb/milvus:latest
  ```

### Traditional Databases
- [ ] **PostgreSQL** - Relational database
  ```bash
  # Ubuntu/Debian
  sudo apt install postgresql postgresql-contrib
  
  # macOS
  brew install postgresql@16
  
  # Docker
  docker run -d --name postgres -p 5432:5432 \
    -e POSTGRES_PASSWORD=postgres \
    postgres:16
  
  psql --version
  ```

- [ ] **MySQL/MariaDB** - Relational database
  ```bash
  # Ubuntu/Debian
  sudo apt install mysql-server
  
  # macOS
  brew install mysql
  
  mysql --version
  ```

- [ ] **SQLite** - Embedded database
  ```bash
  # Usually pre-installed
  sqlite3 --version
  ```

### In-Memory Stores
- [ ] **Redis** - In-memory data structure store
  ```bash
  # Ubuntu/Debian
  sudo apt install redis-server
  
  # macOS
  brew install redis
  
  # Docker
  docker run -d --name redis -p 6379:6379 redis:latest
  
  redis-cli --version
  ```

- [ ] **Memcached** - Distributed memory caching
  ```bash
  # Ubuntu/Debian
  sudo apt install memcached
  
  # macOS
  brew install memcached
  ```

---

## ☁️ Cloud & Infrastructure Tools

### Cloud CLIs
- [ ] **AWS CLI** - Amazon Web Services
  ```bash
  # Ubuntu/Debian/macOS
  curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
  unzip awscliv2.zip
  sudo ./aws/install
  
  aws --version
  ```

- [ ] **Azure CLI** - Microsoft Azure
  ```bash
  # Ubuntu/Debian
  curl -sL https://aka.ms/InstallAzureCLIDeb | sudo bash
  
  # macOS
  brew install azure-cli
  
  az --version
  ```

- [ ] **Google Cloud SDK** - Google Cloud Platform
  ```bash
  # Ubuntu/Debian
  curl https://sdk.cloud.google.com | bash
  
  # macOS
  brew install --cask google-cloud-sdk
  
  gcloud --version
  ```

### Infrastructure as Code
- [ ] **Terraform** - Infrastructure provisioning
  ```bash
  # Ubuntu/Debian
  wget -O- https://apt.releases.hashicorp.com/gpg | sudo gpg --dearmor -o /usr/share/keyrings/hashicorp-archive-keyring.gpg
  echo "deb [signed-by=/usr/share/keyrings/hashicorp-archive-keyring.gpg] https://apt.releases.hashicorp.com $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/hashicorp.list
  sudo apt update && sudo apt install terraform
  
  # macOS
  brew install terraform
  
  terraform --version
  ```

- [ ] **Ansible** - Configuration management
  ```bash
  # Ubuntu/Debian
  sudo apt install ansible
  
  # macOS
  brew install ansible
  
  # Or via pip
  pipx install ansible
  
  ansible --version
  ```

- [ ] **Pulumi** - Infrastructure as code (alternative)
  ```bash
  curl -fsSL https://get.pulumi.com | sh
  pulumi version
  ```

---

## 🪟 Windows PowerShell

- [ ] **PowerShell 7+** - Cross-platform PowerShell
  ```powershell
  # Windows (via winget)
  winget install Microsoft.PowerShell
  
  # Windows (via MSI)
  # Download from: https://github.com/PowerShell/PowerShell/releases
  
  # macOS
  brew install --cask powershell
  
  # Ubuntu/Debian
  wget https://github.com/PowerShell/PowerShell/releases/download/v7.4.0/powershell_7.4.0-1.deb_amd64.deb
  sudo dpkg -i powershell_7.4.0-1.deb_amd64.deb
  
  pwsh --version  # Should show 7.4.0+
  ```

- [ ] **PowerShell Modules**
  ```powershell
  # Install common modules
  Install-Module -Name Az -Scope CurrentUser  # Azure
  Install-Module -Name AWS.Tools.Installer -Scope CurrentUser  # AWS
  Install-Module -Name PSReadLine -Scope CurrentUser  # Improved console
  ```

- [ ] **Windows Terminal** (Windows only)
  ```powershell
  winget install Microsoft.WindowsTerminal
  ```

- [ ] **Scoop** - Windows package manager (alternative to Chocolatey)
  ```powershell
  Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
  irm get.scoop.sh | iex
  ```

- [ ] **Chocolatey** - Windows package manager
  ```powershell
  Set-ExecutionPolicy Bypass -Scope Process -Force
  [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
  iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
  
  choco --version
  ```

---

## 🛠️ Development Tools & IDEs

### Editors & IDEs
- [ ] **Visual Studio Code** - Lightweight code editor
  ```bash
  # Ubuntu/Debian
  sudo snap install code --classic
  
  # macOS
  brew install --cask visual-studio-code
  
  # Windows
  winget install Microsoft.VisualStudioCode
  
  code --version
  ```

- [ ] **VS Code Extensions** (Essential)
  ```bash
  code --install-extension ms-python.python
  code --install-extension ms-vscode.cpptools
  code --install-extension rust-lang.rust-analyzer
  code --install-extension golang.go
  code --install-extension dbaeumer.vscode-eslint
  code --install-extension esbenp.prettier-vscode
  code --install-extension ms-azuretools.vscode-docker
  code --install-extension github.copilot
  ```

- [ ] **JetBrains Toolbox** (Optional)
  ```bash
  # Download from: https://www.jetbrains.com/toolbox-app/
  # Provides: PyCharm, IntelliJ IDEA, WebStorm, CLion, etc.
  ```

### Terminal Emulators
- [ ] **tmux** - Terminal multiplexer
  ```bash
  # Ubuntu/Debian
  sudo apt install tmux
  
  # macOS
  brew install tmux
  
  tmux -V  # 3.4+
  ```

- [ ] **screen** - Terminal multiplexer alternative
  ```bash
  sudo apt install screen  # Usually pre-installed
  ```

- [ ] **iTerm2** (macOS only)
  ```bash
  brew install --cask iterm2
  ```

---

## 🔍 Command-Line Utilities

### Core Utilities
- [ ] **git** - Version control
  ```bash
  # Ubuntu/Debian
  sudo apt install git
  
  # macOS (via Xcode Command Line Tools)
  xcode-select --install
  
  git --version  # 2.49.0+
  ```

- [ ] **GitHub CLI** - GitHub command-line tool
  ```bash
  # Ubuntu/Debian
  type -p curl >/dev/null || sudo apt install curl -y
  curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | sudo dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg
  sudo chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg
  echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | sudo tee /etc/apt/sources.list.d/github-cli.list > /dev/null
  sudo apt update
  sudo apt install gh -y
  
  # macOS
  brew install gh
  
  gh --version
  ```

- [ ] **curl** - Data transfer tool
  ```bash
  curl --version  # 8.5.0+
  ```

- [ ] **wget** - File downloader
  ```bash
  # Ubuntu/Debian
  sudo apt install wget
  
  # macOS
  brew install wget
  
  wget --version
  ```

### Text Processing
- [ ] **awk** - Pattern scanning and processing
  ```bash
  awk --version  # GNU Awk 5.2.1+
  ```

- [ ] **sed** - Stream editor
  ```bash
  sed --version  # GNU sed 4.9+
  ```

- [ ] **grep** - Pattern matching
  ```bash
  grep --version  # GNU grep 3.11+
  ```

- [ ] **ripgrep (rg)** - Fast grep alternative
  ```bash
  # Ubuntu/Debian
  sudo apt install ripgrep
  
  # macOS
  brew install ripgrep
  
  rg --version  # 14.1.0+
  ```

### File Management
- [ ] **tar** - Archive utility
  ```bash
  tar --version  # GNU tar 1.35+
  ```

- [ ] **gzip** - Compression utility
  ```bash
  gzip --version  # 1.12+
  ```

- [ ] **unzip** - ZIP extraction
  ```bash
  sudo apt install unzip  # If not installed
  unzip -v
  ```

- [ ] **7zip** - Advanced archiver
  ```bash
  # Ubuntu/Debian
  sudo apt install p7zip-full
  
  # macOS
  brew install p7zip
  ```

### Data Processing
- [ ] **jq** - JSON processor
  ```bash
  # Ubuntu/Debian
  sudo apt install jq
  
  # macOS
  brew install jq
  
  jq --version  # 1.7+
  ```

- [ ] **yq** - YAML processor
  ```bash
  # Ubuntu/Debian
  sudo wget -qO /usr/local/bin/yq https://github.com/mikefarah/yq/releases/latest/download/yq_linux_amd64
  sudo chmod a+x /usr/local/bin/yq
  
  # macOS
  brew install yq
  
  yq --version
  ```

- [ ] **xmllint** - XML processor
  ```bash
  # Ubuntu/Debian
  sudo apt install libxml2-utils
  
  xmllint --version
  ```

### System Monitoring
- [ ] **htop** - Process viewer
  ```bash
  # Ubuntu/Debian
  sudo apt install htop
  
  # macOS
  brew install htop
  ```

- [ ] **btop** - Resource monitor
  ```bash
  # Ubuntu/Debian
  sudo apt install btop
  
  # macOS
  brew install btop
  ```

- [ ] **ncdu** - Disk usage analyzer
  ```bash
  # Ubuntu/Debian
  sudo apt install ncdu
  
  # macOS
  brew install ncdu
  ```

---

## 📡 AI & Machine Learning Tools

### Local LLM
- [ ] **Ollama** - Run LLMs locally
  ```bash
  # Linux
  curl -fsSL https://ollama.com/install.sh | sh
  
  # macOS
  brew install ollama
  
  # Start Ollama service
  ollama serve
  
  # Pull models
  ollama pull llama3.2
  ollama pull codellama
  ollama pull mistral
  ```

### Python ML Libraries
- [ ] **PyTorch** - Deep learning framework
  ```bash
  pip install torch torchvision torchaudio
  ```

- [ ] **TensorFlow** - Deep learning framework
  ```bash
  pip install tensorflow
  ```

- [ ] **scikit-learn** - Machine learning library
  ```bash
  pip install scikit-learn
  ```

- [ ] **pandas** - Data analysis
  ```bash
  pip install pandas
  ```

- [ ] **numpy** - Numerical computing
  ```bash
  pip install numpy
  ```

---

## 🔐 Security & Authentication

- [ ] **OpenSSL** - Cryptography toolkit
  ```bash
  openssl version  # Usually pre-installed
  ```

- [ ] **GPG** - Encryption and signing
  ```bash
  # Ubuntu/Debian
  sudo apt install gnupg
  
  # macOS
  brew install gnupg
  
  gpg --version
  ```

- [ ] **1Password CLI** - Password manager CLI
  ```bash
  # macOS
  brew install --cask 1password-cli
  
  # Ubuntu/Debian
  # Download from: https://1password.com/downloads/command-line/
  ```

- [ ] **Vault** - Secrets management
  ```bash
  # Ubuntu/Debian
  wget -O- https://apt.releases.hashicorp.com/gpg | sudo gpg --dearmor -o /usr/share/keyrings/hashicorp-archive-keyring.gpg
  echo "deb [signed-by=/usr/share/keyrings/hashicorp-archive-keyring.gpg] https://apt.releases.hashicorp.com $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/hashicorp.list
  sudo apt update && sudo apt install vault
  
  # macOS
  brew install vault
  
  vault --version
  ```

---

## 🧪 Testing & Quality Tools

- [ ] **Postman** - API testing
  ```bash
  # macOS
  brew install --cask postman
  
  # Windows
  winget install Postman.Postman
  
  # Or use web version: https://www.postman.com/
  ```

- [ ] **Newman** - Postman CLI
  ```bash
  npm install -g newman
  ```

- [ ] **Artillery** - Load testing
  ```bash
  npm install -g artillery
  ```

- [ ] **k6** - Load testing
  ```bash
  # Ubuntu/Debian
  sudo gpg -k
  sudo gpg --no-default-keyring --keyring /usr/share/keyrings/k6-archive-keyring.gpg --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys C5AD17C747E3415A3642D57D77C6C491D6AC1D69
  echo "deb [signed-by=/usr/share/keyrings/k6-archive-keyring.gpg] https://dl.k6.io/deb stable main" | sudo tee /etc/apt/sources.list.d/k6.list
  sudo apt-get update
  sudo apt-get install k6
  
  # macOS
  brew install k6
  ```

---

## 📚 Documentation Tools

- [ ] **Sphinx** - Python documentation
  ```bash
  pip install sphinx sphinx-rtd-theme
  ```

- [ ] **MkDocs** - Project documentation
  ```bash
  pip install mkdocs mkdocs-material
  ```

- [ ] **Doxygen** - C/C++ documentation
  ```bash
  # Ubuntu/Debian
  sudo apt install doxygen
  
  # macOS
  brew install doxygen
  ```

- [ ] **Pandoc** - Document converter
  ```bash
  # Ubuntu/Debian
  sudo apt install pandoc
  
  # macOS
  brew install pandoc
  ```

---

## ✅ Verification

After installing all tools, run the verification script:

```bash
# Linux/macOS
./scripts/verify-environment.sh

# Windows
.\scripts\verify-environment.ps1
```

The script will check all installed tools and report which ones are missing or need updates.

---

## 📝 DevTeam6-Specific Setup

After installing the above tools, set up DevTeam6:

```bash
# 1. Clone repository
git clone https://github.com/SpiralCloudOmega/DevTeam6.git
cd DevTeam6

# 2. Copy environment files
cp local-ai/.env.example local-ai/.env
cp app/.env.example app/.env
cp projects/.env.example projects/.env

# 3. Install dependencies
npm install
cd local-ai && pip install -r requirements.txt && cd ..

# 4. Start Qdrant (if using)
docker run -d --name qdrant -p 6333:6333 -p 6334:6334 \
  -v $(pwd)/qdrant_storage:/qdrant/storage:z \
  qdrant/qdrant

# 5. Start Ollama
ollama serve &
ollama pull llama3.2

# 6. Start services
docker-compose up -d
```

---

## 🌟 Optional Enhancements

### Shell Enhancements
- [ ] **oh-my-zsh** - Zsh framework
  ```bash
  sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"
  ```

- [ ] **starship** - Cross-shell prompt
  ```bash
  curl -sS https://starship.rs/install.sh | sh
  ```

- [ ] **fzf** - Fuzzy finder
  ```bash
  # Ubuntu/Debian
  sudo apt install fzf
  
  # macOS
  brew install fzf
  ```

### Additional Tools
- [ ] **lazygit** - Git TUI
  ```bash
  # Ubuntu/Debian
  sudo add-apt-repository ppa:lazygit-team/release
  sudo apt-get update
  sudo apt-get install lazygit
  
  # macOS
  brew install lazygit
  ```

- [ ] **delta** - Better git diff
  ```bash
  # Ubuntu/Debian
  wget https://github.com/dandavison/delta/releases/download/0.16.5/git-delta_0.16.5_amd64.deb
  sudo dpkg -i git-delta_0.16.5_amd64.deb
  
  # macOS
  brew install git-delta
  ```

---

## 🔄 Keep Tools Updated

### Update Commands

**Python**:
```bash
pip install --upgrade pip
pipx upgrade-all
```

**Node.js**:
```bash
nvm install node --reinstall-packages-from=current
npm update -g
```

**System (Ubuntu/Debian)**:
```bash
sudo apt update && sudo apt upgrade
```

**System (macOS)**:
```bash
brew update && brew upgrade
```

**Rust**:
```bash
rustup update
```

**Go**:
```bash
# Download latest from: https://go.dev/dl/
```

---

## 📖 Additional Resources

- [DevTeam6 Contributing Guide](./CONTRIBUTING.md)
- [Deployment Guide](./docs/DEPLOYMENT_GUIDE.md)
- [Quick Reference](./docs/QUICK_REFERENCE.md)
- [Troubleshooting](./docs/TROUBLESHOOTING.md)

---

## 🆘 Getting Help

If you encounter issues during setup:

1. Check the [Troubleshooting Guide](./docs/TROUBLESHOOTING.md)
2. Search existing [GitHub Issues](https://github.com/SpiralCloudOmega/DevTeam6/issues)
3. Ask in [Discussions](https://github.com/SpiralCloudOmega/DevTeam6/discussions)
4. Create a new issue using the [bug report template](.github/ISSUE_TEMPLATE/bug_report.md)

---

**Last Updated**: February 2026
**Maintainer**: DevTeam6 Contributors
