# Oscillator

## 2024-08-01

- There's a small amount of interaction between the PWM dial and PWM
  CV limit due to current flowing between the two potentiometers. This
  can be reduced (but never eliminated completely) by using a larger
  resistor between each potentiometer and the op-amp. Experiment with
  larger resistor values here next time.

## 2024-09-01

- Built this circuit for the second time and it didn't work first try.
- Failure mode is that the triangle wave output starts with a small
  negative voltage and slowly rises. Tried in both OSC and LFO mode,
  same result.
