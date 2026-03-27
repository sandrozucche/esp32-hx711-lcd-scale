ESP32 Professional Scale (HX711 + LCD I2C)

Este repositório contém o firmware para uma balança digital de alta precisão baseada no microcontrolador ESP32, 
utilizando o conversor A/D HX711 e uma interface visual LCD 20x4 via expansor I2C (PCF8574).
O projeto foi desenvolvido nativamente em C utilizando o framework ESP-IDF, seguindo padrões profissionais de organização 
de pastas e modularização de drivers.

🏗️ Status do Projeto: 

Fase de DesenvolvimentoEste projeto é parte integrante do desenvolvimento de uma Balança Digital de Café, 
focada em precisão e estética industrial. 

O projeto está sendo executado em etapas para garantir que a integração entre hardware e software seja perfeita.

📅 Roadmap de Desenvolvimento

- [x] Fase 1: 
* Firmware Core (Concluído)Implementação da comunicação estável com o HX711 via ESP-IDF.
* Desenvolvimento do driver para o display LCD 20x4 I2C.
* Lógica de tara e calibração em tempo real.

- [ ] Fase 2: 
* Prototipagem Eletrônica (Em progresso)Testes de estabilidade de sinal e redução de ruído eletromagnético.
* Definição da alimentação (Bateria Li-ion vs. USB-C).

- [ ] Fase 3: 
* Design Industrial & Modelagem 3D (Próxima etapa)Modelagem do gabinete no Rhino 3D / Autodesk Inventor.
* Foco em ergonomia e durabilidade (EBANISTA approach).
* Design inspirado em equipamentos de áudio vintage.

- [ ] Fase 4: 
* Fabricação & MontagemImpressão 3D de protótipos funcionais.
* Acabamento final e montagem do MVP (Minimum Viable Product).

🚀 FuncionalidadesArquitetura Modular: Drivers de hardware (hx711 e lcd_i2c) separados da lógica principal.Auto-Tara: 

- Sistema de zeragem automática ao inicializar.Leitura Estável: 
* Implementação de média aritmética para filtragem de ruído da célula de carga.

- Interface em Tempo Real: 
* Atualização dinâmica do peso e status no display LCD.
* Gestão de Erros: Detecção de falha de comunicação com o sensor via log e display.

📁 Estrutura do Projeto

Plaintext

├── main/
│   ├── main.c           # Lógica da aplicação e loop principal
│   ├── hx711.c / .h     # Driver do sensor de peso (Protocolo 24-bit)
│   ├── lcd_i2c.c / .h   # Driver do display LCD (Protocolo I2C/4-bit)
│   └── CMakeLists.txt   # Configuração de build do componente
├── CMakeLists.txt       # Configuração principal do projeto
└── .gitignore           # Filtro de arquivos para Git


🛠️ Hardware e Pinagem

Componente, Pino ESP32, Função
HX711,      GPIO 13,    Data (DT)
HX711,      GPIO 14,    Clock (SCK)
LCD I2C,    GPIO 21,    SDA
LCD I2C,    GPIO 22,    SCL

⚖️ Procedimento de Calibração

Para obter leituras precisas, você deve ajustar o calibration_factor no arquivo main.c:
1 - Inicie a balança sem peso (ela fará a tara automática).
2 - Coloque um objeto de peso conhecido (ex: 500g).
3 - Observe a leitura no display.

Se a leitura for diferente de 500g, aplique a fórmula:

$$\text{Novo Fator} = \frac{\text{Leitura Atual} \times \text{Fator Atual}}{\text{Peso Real}}$$

Atualize a variável calibration_factor no código e faça o upload novamente.

⚙️ Como Compilar

Este projeto utiliza o ESP-IDF v4.4 ou superior.

Clone o repositório:
Bashgit clone https://github.com/seu-usuario/esp32-hx711-lcd-scale.git

Inicie o ambiente do IDF e compile:
Bashidf.py build

Grave no ESP32:Bashidf.py -p [PORTA_COM] flash monitor

Desenvolvido por: Sandro ZuccheLicença: MIT