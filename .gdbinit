target extended-remote :3333
monitor reset halt
load
monitor reset init
skip function Timer5IntHandler
b NmiSR
b FaultISR
