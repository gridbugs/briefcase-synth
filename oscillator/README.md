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
- Rebuilt circuit on a breadboard and it worked. With nothing attached
  to the 33340's CV input it still oscillates slowly, so that's not
  the issue.
- Found an issue with the breadboard implementation where the waveform
  didn't look correct. The saw output was bent and the triangle output
  was asymmetric. Replacing the 3340 with a different 3340 fixed this,
  so the original 3340 must have been faulty or damaged.
- For debugging purposes, without the timing cap the circuit still
  oscillates really fast.
- Try without the op-amp in case the op-amp is damaged.
- Rebuilt a stripped-down version of the circuit to make sure that the
  oscillator works in the simplest case and then built up the circuit
  from there. Works now, but still no idea what's wrong with the
  original.
- In the rebuilt circuit there was a short between the negative supply
  and one of the +inputs which seemed to damage a 3340's PWM
  functionality.

## 2024-09-02

- Built front panel. Next time need to move all components further
  from the centre as the potentiometer pins are hard to tessellate
  with each other and the jack sockets.
