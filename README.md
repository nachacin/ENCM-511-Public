# Embedded-Projects
A repository for version control and collaboration for embedded systems projects. These are listed below in order from most recent to least recent.

multimeter.X

This is the latest work. We integrated several modules to implement a multimeter capable of measuring voltage, resistance, capacitance, and frequency. We make use of the PIC24F's ADC, Comparator and Input Capture modules to obtain measurements that are then transmitted through UART in ASCII format. By using a serial terminal, one can monitor readings as buttons are pressed to enter each mode.

voltmeter.X

In this project, we expanded on uart-interface.X by implementing a robust hierarchical state machine with well-defined behaviours. The top-state is governed by the while loop in main.c and the sub-state is governed by the stateMachine_t object, its state transition table, and state functions. In this way, the added voltmeter function can preemptively interrupt the sub-state, execute to completion, and return to the sub-state. The program uses the ADC module to obtain 1000 samples from pin 8, calculates the average, then transmits the result through UART in ASCII format. On a serial terminal, one should be able to see the result's magnitude as a horizontal bar graph which is updated with new results every second for 10 seconds.

uart-interface.X

This was the first attempt to implement a state machine with the PIC24F. We made it a Moore Type FSM and used a simple global variable to hold the machine's state. The validity of the state transitions was tested with messages sent through UART and read on a serial terminal.
