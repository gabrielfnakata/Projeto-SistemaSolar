# 🌌 Sistema Solar 3D

Um simulador interativo do Sistema Solar desenvolvido em C++ com OpenGL, apresentando todos os planetas com texturas realistas, movimento orbital e rotacional, além de um modo noturno com estrelas cintilantes.

![Sistema Solar 3D](https://img.shields.io/badge/OpenGL-3D-blue)
![C++](https://img.shields.io/badge/C++-17-green)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-lightgrey)

## 🧑‍🎓 Alunos:

- Arthur Pacheco - SP3123022
- Gabriel Nakata - SP3119548
- Glauco Condo   - SP3123073

## 🎯 Controles

| Controle | Ação |
|----------|------|
| **Mouse Esquerdo + Arrastar** | Rotacionar câmera |
| **Scroll Mouse** | Zoom in/out |
| **Espaço** | Pausar/Retomar animação |
| **N** | Ativar/Desativar modo noturno |
| **ESC** | Sair do programa |

## 🛠️ Instalação e Compilação

### Pré-requisitos

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

#### Windows
- Visual Studio com suporte C++
- OpenGL libraries
- FreeGLUT

### 📁 Estrutura do Projeto
```
Sistema_Solar/
├── include/
│   ├── stb_image.h      # Biblioteca para carregar texturas
│   └── espaço.wav       # Som ambiente (opcional)
├── texturas/
│   ├── sol.jpg          # Textura do Sol
│   ├── lua.jpg          # Textura da Lua
│   ├── mercurio.jpg     # Textura de Mercúrio
│   ├── venus.jpg        # Textura de Vênus
│   ├── terra.jpg        # Textura da Terra
│   ├── marte.jpg        # Textura de Marte
│   ├── jupiter.jpg      # Textura de Júpiter
│   ├── saturno.jpg      # Textura de Saturno
│   ├── urano.jpg        # Textura de Urano
│   └── netuno.jpg       # Textura de Netuno
└── sistema_solar.cpp    # Código principal
```

### 🚀 Compilação e Execução

#### Linux
```bash
# Clone o repositório
git clone [seu-repositorio]
cd Sistema_Solar

# Compile o projeto
g++ -o sistema_solar sistema_solar.cpp -lGL -lGLU -lglut -lm

# Execute
./sistema_solar
```

#### Windows
```bash
# Com Visual Studio
cl sistema_solar.cpp opengl32.lib glu32.lib freeglut.lib

# Execute
sistema_solar.exe
```

## 🎨 Recursos Técnicos

### 🖼️ Sistema de Texturas
- **Carregamento**: Utiliza biblioteca STB para suporte a JPG/PNG
- **Mapeamento**: Coordenadas UV automáticas nas esferas
- **Qualidade**: Filtragem linear para suavidade

### 🌍 Física Simplificada
- **Velocidades Orbitais**: Baseadas em dados reais (escalonadas)
- **Rotação Axial**: Cada planeta com sua velocidade de rotação
- **Inclinação**: Eixos planetários com inclinações realistas

### 💫 Modo Noturno
- **500 Estrelas**: Distribuídas aleatoriamente em esfera
- **Cintilação**: Efeito de brilho variável no tempo
- **Atmosfera**: Fundo azul escuro simulando espaço profundo

## 🔧 Dependências

- **OpenGL** - Renderização gráfica 3D
- **GLUT/FreeGLUT** - Janelas e controles
- **GLU** - Utilitários OpenGL (esferas)
- **STB Image** - Carregamento de texturas
- **C++ Standard Library** - Funcionalidades básicas

## 📊 Dados dos Planetas

| Planeta | Raio Relativo | Distância | Velocidade Orbital | Luas |
|---------|---------------|-----------|-------------------|------|
| Mercúrio | 0.2 | 4.0 | 4.0 | ❌ |
| Vênus | 0.6 | 5.6 | 1.6 | ❌ |
| Terra | 0.36 | 7.2 | 1.0 | ✅ |
| Marte | 0.28 | 17.6 | 0.53 | ✅ |
| Júpiter | 0.8 | 12.0 | 0.084 | ✅ |
| Saturno | 0.7 | 14.8 | 0.034 | ✅ |
| Urano | 0.6 | 17.6 | 0.012 | ✅ |
| Netuno | 0.6 | 20.4 | 0.006 | ✅ |
