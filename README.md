# ButtonDebounceRepeat

ButtonDebounceRepeat is an Arduino library for button handling. It provides debouncing, click counting, hold detection, and multi-speed repeat events for a single button input.

## Features

- Debounced button press and release detection
- Click counting (single, double, etc.)
- Hold detection with multi-speed repeat events
- Simple API: just call `checkButton()` regularly in your loop

## Usage

1. Copy [`button.h`](button.h) and [`button.cpp`](button.cpp) into your Arduino project or library folder.
2. Include the header in your sketch:

    ````cpp
    #include "button.h"
    ````

3. Create a `Button` instance, passing the pin number:

    ````cpp
    Button myButton(2); // Use digital pin 2
    ````

4. In your `loop()`, call `checkButton()` and handle events:

    ````cpp
    void loop() {
        buttonevent ev = myButton.checkButton();
        // Handle the event, e.g.:
        if (ev & B_PRESS) {
            // Button pressed
        }
        if (ev & B_HOLD) {
            // Button held (with repeat)
        }
        if ((ev & B_TIMEOUT) && ((ev & B_COUNT) == 1)) {
            // Single click complete
        }
    }
    ````

## Event Reference

Events are encoded as bitfields in the `buttonevent` type. See [`button.h`](button.h) for details.

- `B_PRESS` – Button pressed
- `B_RELEASE` – Button released
- `B_HOLD` – Button held (with repeat)
- `B_TIMEOUT` – End of click sequence
- `B_COUNT` – Number of clicks (1–15)

## License

This project is licensed under the GNU GPL v3. See [LICENSE](LICENSE) for details.

## Credits

- Concept based on [ToyKeeper/anduril](https://github.com/ToyKeeper/anduril)
- Implementation by Teemo Vaas (2024–2025)
