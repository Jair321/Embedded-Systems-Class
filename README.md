# README – ENGR 355 Labs 2–9  
**Quarter:** Winter 2024  
**Instructor:** Dr. Natalie Smith-Gray  
**Course:** Embedded Systems Design – Walla Walla University

## About the Class  
This class taught us how to build and program microcontroller-based systems from the ground up. We used ARM Cortex-M boards and focused heavily on real-world hardware/software integration—things like GPIO, timers, interrupts, ADC/DAC, and LCDs. It was hands-on, technical, and very relevant to how embedded systems are used in everything from sensors to consumer electronics.

---

## Lab Highlights

### **Lab 2 – GPIO**  
We learned how to set up digital input and output pins. This was the foundation—blinking LEDs, reading button presses, and just getting used to how the microcontroller communicates with external hardware. LEDs were turned turn on in the order of LED8, LED9, LED10 (one at a time), and then repeat.

### **Lab 3 – LCD Display**  
Got a character LCD working with the board. This was our first interface-heavy lab, teaching us timing and data transmission.

### **Lab 4 – Button Interrupts**  
We moved beyond polling and wrote interrupt service routines (ISRs) to respond instantly to button presses. This introduced the idea of asynchronous inputs.

### **Lab 5 – Interrupt Priorities**  
We added complexity by handling multiple interrupts and prioritizing them. Helpful for understanding how real systems avoid conflicts.

### **Lab 6 – Timers**  
Used the hardware timers to generate delays and periodic functions. Great intro to precise time-based control, a big part of real-time systems.

### **Lab 7 – DAC (Digital-to-Analog)**  
We output analog voltages from the microcontroller and visualized them with an oscilloscope. A good bridge between digital logic and real-world signals.

### **Lab 8 – ADC (Analog-to-Digital)**  
Took analog input signals (like from sensors) and converted them to digital values. We started to “sense” the environment here.

### **Lab 9 – Board Assembly & Signal Analysis**  
We assembled our custom boards and connected a signal generator to the ADC, output the signal through DAC, and displayed the measured period on an LCD. This brought a lot of concepts together.

---

## Final Thoughts  
This series of labs built up our skills from basic I/O all the way to full-system integration. Every lab felt like it added another building block toward creating a working embedded system. By the end, I felt comfortable debugging hardware, writing embedded C, and understanding how the pieces all fit together.
