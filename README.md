# PXN LFU Linux Firmware (Virtualization-Based Input Workaround)

---

## 🇺🇸 English

⚠️ **Experimental Project Notice**
This project is experimental and not fully stable. Behavior may vary depending on specific PXN hardware revisions and system configurations. Certain edge cases and device-specific quirks may not yet be handled correctly.

### Overview

This project provides a low-level workaround for PXN controllers on Linux systems by leveraging input virtualization techniques to normalize and remap device behavior.

Certain PXN devices expose inconsistent or non-standard input reports under Linux, particularly when interfacing with software relying on SDL2 (Simple DirectMedia Layer 2), such as games and Wine-based applications. This often results in improper device recognition, missing inputs, or incorrect button/axis mappings.

This "firmware" is not a traditional hardware-level firmware. Instead, it operates as a user-space abstraction layer that intercepts raw input events and re-emits them through a virtualized input device with standardized semantics.

### Technical Approach

The solution is based on the following pipeline:

1. **Raw Input Capture**

   * Interfaces directly with `/dev/input/event*`
   * Captures low-level evdev events from the PXN device

2. **Input Normalization Layer**

   * Translates vendor-specific or malformed input data
   * Fixes axis ranges, button mappings, and inconsistencies

3. **Virtual Device Injection**

   * Uses the Linux **uinput** subsystem
   * Creates a virtual controller with standardized behavior

4. **SDL2 Compatibility Layer**

   * Ensures correct detection by SDL2-based software
   * Works with:

     * Native Linux games
     * Wine / Proton environments

### Key Features

* Improved SDL2 compatibility
* Works with Wine and Proton
* Real-time input translation
* No kernel patches required
* Fully user-space implementation

### Limitations

* Experimental and not fully reliable
* Hardware-dependent behavior (PXN models may differ)
* Requires background execution
* Needs proper permissions for `/dev/input/event*`
* May require custom udev rules

### Conclusion

This project acts as a compatibility bridge for PXN controllers on Linux by virtualizing input behavior. While not perfect, it provides a practical workaround for improving controller recognition in environments where native support is lacking.

---

## 🇧🇷 Português

⚠️ **Aviso: Projeto Experimental**
Este projeto é experimental e não está totalmente estável. O comportamento pode variar dependendo da revisão do hardware PXN e da configuração do sistema. Algumas peculiaridades e casos específicos ainda podem não ser tratados corretamente.

### Visão Geral

Este projeto fornece um workaround de baixo nível para controles PXN em sistemas Linux, utilizando técnicas de virtualização de entrada para normalizar e corrigir o comportamento do dispositivo.

Certos dispositivos PXN apresentam relatórios de entrada inconsistentes ou fora do padrão no Linux, especialmente ao interagir com softwares que utilizam SDL2 (Simple DirectMedia Layer 2), como jogos e aplicações via Wine. Isso pode resultar em falhas de reconhecimento, entradas ausentes ou mapeamentos incorretos de botões e eixos.

Este "firmware" não é um firmware tradicional de hardware. Em vez disso, funciona como uma camada de abstração em espaço de usuário que intercepta eventos de entrada brutos e os reemite através de um dispositivo virtual com comportamento padronizado.

### Abordagem Técnica

A solução segue o seguinte fluxo:

1. **Captura de Entrada Bruta**

   * Interface direta com `/dev/input/event*`
   * Captura eventos evdev do controle PXN

2. **Camada de Normalização**

   * Tradução de dados específicos do fabricante
   * Correção de eixos, botões e inconsistências

3. **Injeção de Dispositivo Virtual**

   * Utiliza o subsistema **uinput** do Linux
   * Cria um controle virtual padronizado

4. **Compatibilidade com SDL2**

   * Garante reconhecimento correto por aplicações
   * Compatível com:

     * Jogos nativos Linux
     * Wine / Proton

### Principais Características

* Melhor compatibilidade com SDL2
* Funciona com Wine e Proton
* Tradução de entrada em tempo real
* Não requer modificações no kernel
* Implementação totalmente em espaço de usuário

### Limitações

* Projeto experimental e não totalmente confiável
* Comportamento dependente do hardware (variações entre modelos PXN)
* Necessita execução em segundo plano
* Requer permissões para `/dev/input/event*`
* Pode exigir regras udev personalizadas

### Conclusão

Este projeto atua como uma camada de compatibilidade para controles PXN no Linux através da virtualização da entrada. Apesar de não ser uma solução perfeita, oferece um workaround prático para melhorar o reconhecimento do controle em ambientes onde o suporte nativo é limitado.
