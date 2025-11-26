/**
 * TypeScript definitions for xinput-napi
 */

export interface XInputButtons {
  dpadUp: boolean;
  dpadDown: boolean;
  dpadLeft: boolean;
  dpadRight: boolean;
  start: boolean;
  back: boolean;
  leftThumb: boolean;
  rightThumb: boolean;
  leftShoulder: boolean;
  rightShoulder: boolean;
  a: boolean;
  b: boolean;
  x: boolean;
  y: boolean;
}

export interface XInputStick {
  x: number;  // -1.0 to 1.0
  y: number;  // -1.0 to 1.0
}

export interface XInputGamepad {
  buttons: XInputButtons;
  leftTrigger: number;   // 0.0 to 1.0
  rightTrigger: number;  // 0.0 to 1.0
  leftStick: XInputStick;
  rightStick: XInputStick;
  packetNumber: number;
}

export interface XInputState {
  connected: boolean;
  gamepad?: XInputGamepad;
}

export interface XInputBatteryInfo {
  type?: 'Disconnected' | 'Wired' | 'Alkaline' | 'NiMH' | 'Unknown';
  level?: 'Empty' | 'Low' | 'Medium' | 'Full' | 'Unknown';
}

/**
 * Get the current state of a controller
 * @param index Controller index (0-3)
 * @returns Controller state with connected status and gamepad data
 */
export function getControllerState(index: number): XInputState;

/**
 * Get indices of all connected controllers
 * @returns Array of connected controller indices (e.g., [0, 1])
 */
export function getConnectedControllers(): number[];

/**
 * Set controller vibration (rumble)
 * @param index Controller index (0-3)
 * @param leftMotor Left motor intensity (0.0 to 1.0)
 * @param rightMotor Right motor intensity (0.0 to 1.0)
 * @returns true if successful
 */
export function setVibration(index: number, leftMotor: number, rightMotor: number): boolean;

/**
 * Check if XInput is available on this platform
 * @returns true on Windows, false on macOS/Linux
 */
export function isAvailable(): boolean;

/**
 * Get battery information for a controller
 * @param index Controller index (0-3)
 * @returns Battery type and level
 */
export function getBatteryInfo(index: number): XInputBatteryInfo;

