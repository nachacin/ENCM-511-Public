# Embedded-Projects
A repository for version control and collaboration for ENCM 511 projects developed by group 4.

Check out voltmeter.X for the latest work. We expanded on uart-interface.X by implementing a robust hierarchical state machine with well-defined behaviors. The program uses the ADC module to obtain 1000 samples from pin 8, calculates the average, then transmits the result through UART in ASCII format. On a serial terminal, one should be able to see the result's magnitude as a horizontal bar graph which is updated with new results every second for 10 seconds.
