# Mac Display Utility

A macOS command line tool to manage an arrangement
and configuration of your displays. A minimalistic alternative
and direct derivative of (apparently) no longer maintained
[displayplacer](https://github.com/jakehilborn/displayplacer).

## Usage

1. Go to System Preferences > Displays. Arrange your displays as desired.
1. Save your current display configuration by running:

    ```bash
    macd save-configuration <name>
    ```

1. Repeat steps 1-2 for every display configuration you need.
1. Apply any saved configuration by running:

    ```bash
    macd apply-configuration <name>
    ```

Configurations can be also edited manually via `~/.macd.json` file.

## Commands

- `macd list-displays` - list online displays and their properties
(origin, width, height, is built-in, is main).
- `macd list-configurations` - list saved configurations.
- `macd save-configuration <name>` - save the current configuration
of displays under `<name>`.
- `macd apply-configuration <name>` - apply the saved configuration.

## Installation

Brew Formula to be created soon.

## Compilation

```bash
make
```

## Contribution

Please create a GitHub issue for any feedback, bugs,
feature requests or issues. PRs are also welcome.
