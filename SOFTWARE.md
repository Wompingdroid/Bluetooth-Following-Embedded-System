flowchart TD
    A[Bluetooth beacon transmitter] --> B(Send out beacon signal every 100 ms)

    RECV1[Recieves bluetooth signal] --> |Sends averaged RSSI every second| MAIN_PROC
    RECV2[Recieves bluetooth signal] --> |Sends averaged RSSI every second| MAIN_PROC
    MAIN_PROC(Performs triangulation with RSSI info) --> |Sends control signals to motors| MOTORS(Motors driver)
