#!/usr/bin/env bash

set -euo pipefail

./scripts/build-fw.sh

# openocd: talks to debug probes (ST-Link, J-Link) to program/debug MCUs
#   - it starts multiple TCP/IP servers when launched:
#     - GDB server (3333): 
#       - input debug packets translated to SWD protocol ops on chip
#       - ST-Link HW physically clocks these ops onto the SWDIO/SWCLK pins (transmission)
#       - nucleo only supports SWD (not JTAG)
       
#     - Telnet server (4444) 
#       - listens for manual commands to flash/inspect targets
#       - `telnet localhost 4444`: can use OpenOCD commands directly (halt, resume, reg pc, etc.)
#       - useful for ad-hoc debugging without GDB

#     - Tcl RPC (6666): 
#       - programming control (scripts) can connect and send Tcl commands/get responses
#       - used in CI, test rigs, custom tooling

# openocd: start an openocd process 
    # -f board/st_nucleo_f4.cfg: specify config file (a built-in OpenOCD config file for any nucleo-f4 board)
    #   - interface/stlink_cfg: how to talk to ST-Link over USB (SWD protocol)
      #   - target/stm32f4x.cfg : debug architecture for the stm32f4 chipset 
    
    # -c: run commands locally over Tcl interpreter inside the openocd process
    #   - program build/fw/fw_hover: 
    #     - parses .elf 
    #     - extracts loadable sections,
    #     - sends bytes via SWD to chip's flash controller
    #
    #   - verify: read flash back over SWD and compare with .elf
    
    #   - reset: pulse reset line via SWD
    #     - CPU in halted state after flashing
    #     - reset starts FW from scratch: load vector table, run reset handler and execute main()
    
    #   - exit: close USB connection, shut down TCP servers, terminate
    #     - free ST-Link proble and shell prompt
    
    
openocd -f board/st_nucleo_f4.cfg \
        -c "program build/fw/fw_hover verify reset exit"


