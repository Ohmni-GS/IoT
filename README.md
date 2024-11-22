# OHMNI - Plataforma de Gerenciamento Energético Sustentável  

---

## Índice  
1. [Descrição do Projeto](#descrição-do-projeto)  
2. [O Dispositivo IoT](#o-dispositivo-iot)  
   - [Componentes Necessários](#componentes-necessários)  
   - [Funcionalidades](#funcionalidades)  
3. [Configuração e Uso do Dispositivo](#configuração-e-uso-do-dispositivo)  
4. [Simulação com PlatformIO e Wokwi](#simulação-com-platformio-e-wokwi)  
5. [Conectando-se ao Sistema Web OHMNI](#conectando-se-ao-sistema-web-ohmni)  
6. [Sobre os Integrantes do Grupo](#sobre-os-integrantes-do-grupo)  

---

## Descrição do Projeto  
**OHMNI** é um ecossistema inovador para o gerenciamento eficiente de energia solar em comunidades remotas ou carentes.  

Por meio de uma plataforma Web integrada a um dispositivo IoT, os usuários podem monitorar o consumo energético de maneira centralizada, tanto para comunidades inteiras quanto para moradores individuais. Isso incentiva o uso eficiente de energia sustentável e facilita o acesso à tecnologia em regiões vulneráveis.  

Nosso objetivo é promover soluções sustentáveis e acessíveis, unindo inovação e impacto social.  

---

## O Dispositivo IoT  
O dispositivo OHMNI coleta dados de energia provenientes de sistemas de painéis solares com conversores AC/DC. Ele mede corrente e tensão em sistemas de corrente alternada (AC) e envia as informações ao servidor em tempo real via **MQTT**.  

### Componentes Necessários  
- **ESP32**: Microcontrolador principal para o dispositivo.  
- **Display LCD 16x2 (I2C)**: Usado para exibir informações em tempo real.  
- **Sensor de Tensão AC - ZMPT101B**: Substituído por um potenciômetro na simulação.  
- **Sensor de Corrente Não Invasivo SCT-013**: Substituído por um potenciômetro na simulação.  

### Funcionalidades  
- **Conexão com Wi-Fi**: Permite o envio e recebimento de dados pela rede local.  
- **Envio de dados via MQTT**: Protocolo usado para transmitir informações ao servidor em tempo real.  
- **Criação e gerenciamento de IDs únicos para dispositivos**: Cada dispositivo é identificado de forma exclusiva no sistema.  
- **Monitoramento em tempo real do consumo de energia**: Dados exibidos e analisados instantaneamente.  

### Dependências  
As dependências necessárias já estão configuradas no arquivo `platformio.ini`. No entanto, aqui estão as bibliotecas utilizadas:  

```
PubSubClient @ 2.8.0
ArduinoJson @ 7.2.0
LiquidCrystal_I2C @ 1.1.4
Preferences @ 2.0.0
WiFi @ 2.0.0
WiFiClientSecure @ 2.0.0
```  

---

## Configuração e Uso do Dispositivo  

### Pré-requisitos  
- Componentes mencionados na seção anterior.  
- Conexão Wi-Fi disponível.  

### Passos para Configurar o Dispositivo  

#### 1. Conectar os Sensores e Display ao ESP32  
Monte os componentes no ESP32 de acordo com o [diagrama de ligação](./diagram.json).  

#### 2. Configurar Wi-Fi no Código  
No arquivo `esp32-http-server.ino`, configure o SSID e a senha da rede:  
```c
const char *SSID = "Wokwi-GUEST";  
const char *PASSWORD = "";  
```  

#### 3. Enviar o Código para o ESP32  
Compile e envie o código para o ESP32 utilizando o Arduino IDE ou o PlatformIO.  

#### 4. Iniciar o Dispositivo  
Após enviar o código, ligue o dispositivo e verifique se ele se conecta à rede Wi-Fi.  

---

## Simulação com PlatformIO e Wokwi  

Caso não possua os componentes físicos, você pode simular o dispositivo utilizando o [Wokwi Simulator](https://wokwi.com/projects/414450283919840257) integrado ao **PlatformIO** no Visual Studio Code.  

### Pré-requisitos  
- Git instalado.  
- **PlatformIO** instalado no VS Code.  
- Extensão do Simulador **Wokwi** configurada.  

### Passos para Simular 

#### Adendo: Caso queira tentar simular diretamente no site do Wokwi, <a href="https://wokwi.com/projects/414450283919840257">clique aqui</a>, mas é recomendável simular pelo VS Code, uma vez que o Wokwi está crashando repetidamente por timeout na hora da build.

#### 1. Clone o repositório
Para clonar o repositório localmente, use:  
```bash
git clone https://github.com/Ohmni-GS/IoT.git
```  
#### 2. Configurar Wi-Fi no Código  
Edite o arquivo `esp32-http-server.ino` e configure:  
```c
const char *SSID = "Wokwi-GUEST";  
const char *PASSWORD = "";  
```  

#### 3. Compilar o Código  
Abra o código no VS Code e agurde o Platform IO, clique no botão **Build** no canto superior direito.  

#### 4. Executar a Simulação  
Abra o arquivo `diagram.json` com a extensão do Wokwi e execute a simulação. Você verá mensagens no monitor serial indicando o envio de dados ao servidor.  

---

## Conectando-se ao Sistema Web OHMNI  

Apesar da lógica e criação do dispositivo IoT e a API estarem completas e seguirem uma lógica consistente, para o sistema Web foi desenvolvido uma versão simplificada da ideia final.

### Passos para Conectar ao Sistema  

#### 1. Acesse o Sistema  
Clique no link [OHMNI Web](#) e faça o login com dados fictícios.  

#### 2. Conecte o Dispositivo  
No painel Web, clique no botão "Conectar". Os dados do dispositivo serão exibidos em tempo real.  

---

## Integrantes do Grupo  

- **Matheus Queiroz Zanutin** - RM558801  
- **Marcela Torro** - RM557658  
- **Matheus Vinícius** - RM555177  

Desenvolvemos o projeto OHMNI com o propósito de promover acesso sustentável à energia, impactando positivamente comunidades vulneráveis e criando soluções inovadoras para o futuro.  
