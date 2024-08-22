# Filter

## 2024-08-23

- There's a failure mode where the output is silent and one of the
  resonance LEDs is solid on and the other is solid off. This seems to
  mean that the TL074 is damaged. It seems that this circuit damages
  the TL074 when certain pins are connected by accident. There's also
  a failure mode that damages the LM13700. Not sure what causes that.
- Next time don't bother including the high-pass filter. It
  complicates the circuit and UI, is not likely to be used, and
  doesn't work particularly well. Build a dedicated high-pass filter
  if necessary.
- The circuit has a bug where the volume of the self-oscillation
  signal drops quickly above a certain frequency. This has the effect
  of limiting the effect of resonance. Not sure whether this is a bug
  in the design or implementation.
- The stripboard is slightly too large to comfortably fit in the case.
- Using a safety pin to group cables together and drilling a hole in
  the stripboard for cable management was effective at reducing the
  tangle of cables and made it easy to work on the circuit after
  starting work on the front panel.
- Build a subset of the circuit first. Use self-oscillation to test
  the circuit incrementally as it's built.
