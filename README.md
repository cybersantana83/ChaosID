# ChaosID

Perito forense de cartoes pro Flipper Zero. Faz scan sequencial LF (125 kHz) -> HF (13.56 MHz), identifica o protocolo e devolve um dossie com tecnologia, criptografia, vetor de ataque conhecido, ano da quebra e nivel de risco.

Parte da serie **Chaos IAM** | feito pela **Cafe com Solda**.

## Status

v0.1 - POC funcional com scan simulado. Toda a UI, scene manager, base de cartoes e fluxo end-to-end estao prontos. O que falta e plugar os workers reais de RF (marcados com `TODO(integracao)` em `scenes/chaos_id_scene_scanning.c`).

## Estrutura

```
chaos_id/
├── application.fam          manifesto Flipper
├── chaos_id.c               entry point + alloc/free do app
├── chaos_id_app.h           struct de estado + enums (scenes/views/events)
├── scenes/
│   ├── chaos_id_scene.h     declaracoes dos handlers
│   ├── chaos_id_scene.c     tabela de dispatch
│   ├── chaos_id_scene_splash.c     menu principal
│   ├── chaos_id_scene_scanning.c   ponto de integracao com radio
│   ├── chaos_id_scene_result.c     dossie do cartao
│   └── chaos_id_scene_about.c
├── database/
│   ├── cards_db.h           CardProfile struct + helpers
│   └── cards_db.c           15 entries reais (LF + HF)
└── assets/
    └── icon.png             10x10 mono (incluso)
```

## Build (modo isolado, contra SDK do Momentum)

Setup de primeira vez (cria SDK project-local em `.ufbt/` dentro da pasta, em vez do `~/.ufbt/` global):

```bash
# Instala ufbt isolado via pipx (Ubuntu 23.04+ / Debian 12+ exigem por causa do PEP 668)
sudo apt install pipx
pipx ensurepath && exec $SHELL
pipx install ufbt

cd chaos_id
ufbt dotenv_create                              # SDK fica em .ufbt/ DENTRO do projeto
ufbt update --index-url=https://up.momentum-fw.dev/firmware/directory.json
ufbt vscode_dist                                # gera .vscode/ com tasks + IntelliSense
code .
```

Ciclo de desenvolvimento:

```bash
ufbt           # build -> dist/chaos_id.fap
ufbt launch    # build + flash USB + executa no Flipper conectado
ufbt cli       # console serial do Flipper (qFlipper precisa estar FECHADO)
ufbt -c        # clean
```

No VS Code: **Ctrl+Shift+B** dispara build. Pra step-debug precisa de probe SWD; sem probe, usa `furi_log_*` no codigo e olha o output no `ufbt cli`.

Antes do `ufbt launch`, fecha qFlipper e a aba do lab.flipper.net se estiverem abertos - seguram a porta USB.

O `assets/icon.png` ja vem no scaffold (10x10 1-bit, cartao com chip EMV). Pra trocar o desenho, qualquer PNG 10x10 monocromatico serve.

## Como o scan funciona hoje (POC)

`scenes/chaos_id_scene_scanning.c` orquestra duas fases sequenciais com timer:

1. **Fase LF** (2.5s): tela "Lendo LF (125 kHz)..." -> sorteio (30% hit) -> se hit, vai pro dossie. Se nao, parte pra fase HF.
2. **Fase HF** (2.5s): tela "Lendo HF (13.56 MHz)..." -> sorteio (70% hit) -> dossie ou tela de "nada detectado".

O dossie e renderizado em `chaos_id_scene_result.c` usando `widget_add_text_scroll_element` (scroll vertical com a roda).

## Plugando RF real (proximo passo)

No `scenes/chaos_id_scene_scanning.c`, substitui os timers + sorteios pelos workers do firmware:

**LF** - usa `lib/lfrfid`:
```c
#include <lib/lfrfid/lfrfid_worker.h>

LFRFIDWorker* lf_worker = lfrfid_worker_alloc(protocols_dict);
lfrfid_worker_start_thread(lf_worker);
lfrfid_worker_read_start(
    lf_worker, LFRFIDWorkerReadTypeAuto, lf_callback, app);
```

O `lf_callback` recebe `LFRFIDWorkerReadResult` e o nome do protocolo detectado. Cruza com `cards_db` por string match no campo `protocol` e seta `app->last_result`.

**HF** - usa `lib/nfc`:
```c
#include <lib/nfc/nfc.h>
#include <lib/nfc/nfc_poller.h>
#include <lib/nfc/protocols/iso14443_3a/iso14443_3a_poller.h>

Nfc* nfc = nfc_alloc();
NfcPoller* poller = nfc_poller_alloc(nfc, NfcProtocolIso14443_3a);
nfc_poller_start(poller, hf_callback, app);
```

Pro MIFARE Classic, depois do detect basico em 14443-3a, instancia `nfc_poller_alloc(nfc, NfcProtocolMfClassic)` pra refinar (1K vs 4K, plus, etc).

UID real vem dos dados retornados pelo poller - escreve em `app->uid_buffer` com `snprintf` igual ja faz o `generate_fake_uid` na POC.

## Roadmap

- **v0.2** persistencia: salva cada scan em `/ext/apps_data/chaos_id/log.csv` (timestamp, protocol, uid, risk).
- **v0.3** botao "Investigar" no dossie de cartoes broken: pra MIFARE Classic chama nested attack direto.
- **v0.4** i18n EN/PT, splash com identidade visual Cafe com Solda, submit pro app catalog oficial.

## Licenca

**GPLv3 ou posterior** (GNU General Public License v3.0-or-later).

- Texto completo em `LICENSE` (canonico, SHA-256 `3972dc97...86`).
- Cada arquivo fonte tem header SPDX `GPL-3.0-or-later`.
- Forks e derivados devem ser redistribuidos sob a mesma licenca.

Escolha alinhada com a cultura do firmware Flipper e de ferramentas de pesquisa em seguranca (Proxmark, Chameleon, etc).
