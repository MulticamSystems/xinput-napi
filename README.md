# xinput-napi

Native XInput bindings for Node.js and Electron. Provides access to Xbox controllers on Windows using Microsoft's XInput API.

## Installation

```bash
npm install @aspect/xinput-napi
```

## Usage

```javascript
const xinput = require('@aspect/xinput-napi');

// Check if XInput is available (Windows only)
if (xinput.isAvailable()) {
  // Get connected controllers
  const controllers = xinput.getConnectedControllers();
  console.log('Connected:', controllers); // e.g., [0, 1]

  // Get controller state
  const state = xinput.getControllerState(0);
  if (state.connected) {
    console.log('Left stick:', state.gamepad.leftStick);
    console.log('Buttons:', state.gamepad.buttons);
    console.log('Triggers:', state.gamepad.leftTrigger, state.gamepad.rightTrigger);
  }

  // Set vibration (rumble)
  xinput.setVibration(0, 0.5, 0.5); // 50% on both motors
}
```

## API

### `isAvailable(): boolean`
Returns `true` on Windows, `false` on other platforms.

### `getConnectedControllers(): number[]`
Returns array of connected controller indices (0-3).

### `getControllerState(index: number): XInputState`
Returns the current state of a controller:
```typescript
{
  connected: boolean;
  gamepad?: {
    buttons: {
      a, b, x, y: boolean;
      leftShoulder, rightShoulder: boolean;
      leftThumb, rightThumb: boolean;
      start, back: boolean;
      dpadUp, dpadDown, dpadLeft, dpadRight: boolean;
    };
    leftTrigger: number;   // 0.0 - 1.0
    rightTrigger: number;  // 0.0 - 1.0
    leftStick: { x: number, y: number };   // -1.0 to 1.0
    rightStick: { x: number, y: number };  // -1.0 to 1.0
    packetNumber: number;  // For change detection
  }
}
```

### `setVibration(index: number, leftMotor: number, rightMotor: number): boolean`
Set controller vibration. Motor values are 0.0 to 1.0.

### `getBatteryInfo(index: number): XInputBatteryInfo`
Get battery information for wireless controllers.

## Platform Support

| Platform | Support |
|----------|---------|
| Windows  | ✅ Full XInput support |
| macOS    | ❌ Use [node-hid](https://github.com/node-hid/node-hid) instead |
| Linux    | ❌ Use [node-hid](https://github.com/node-hid/node-hid) instead |

## Building from Source

```bash
npm install
npm run build
```

### Building Prebuilds

```bash
# For Node.js
npm run prebuild

# For Electron
npm run prebuild-electron
```

## License

MIT

