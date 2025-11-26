/**
 * xinput-napi - Native XInput bindings for Node.js/Electron
 * 
 * On Windows: Uses XInput API for Xbox controller access
 * On macOS/Linux: Returns stub (use node-hid instead)
 */

const path = require('path');

let binding;

try {
  // Try to load using node-gyp-build (handles prebuilds and local builds)
  binding = require('node-gyp-build')(path.join(__dirname));
} catch (err) {
  // Fallback: try to load from standard build location
  try {
    binding = require('./build/Release/xinput.node');
  } catch (err2) {
    console.error('xinput-napi: Failed to load native module');
    console.error('  Primary error:', err.message);
    console.error('  Fallback error:', err2.message);
    
    // Return stub module on failure
    binding = {
      getControllerState: () => ({ connected: false }),
      getConnectedControllers: () => [],
      setVibration: () => false,
      isAvailable: () => false,
      getBatteryInfo: () => ({})
    };
  }
}

module.exports = binding;

