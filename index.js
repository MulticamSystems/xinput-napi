/**
 * xinput-napi - Native XInput bindings for Node.js/Electron
 * 
 * On Windows: Uses XInput API for Xbox controller access
 * On macOS/Linux: Returns stub (use node-hid instead)
 */

const path = require('path');
const fs = require('fs');

let binding;

function tryLoad() {
  const prebuildsDir = path.join(__dirname, 'prebuilds', `${process.platform}-${process.arch}`);
  
  // Try prebuild files (multiple naming conventions)
  const prebuildFiles = [
    path.join(prebuildsDir, '@aspect+xinput-napi.node'),
    path.join(prebuildsDir, 'xinput-napi.node'),
    path.join(prebuildsDir, 'xinput.node'),
  ];
  
  for (const file of prebuildFiles) {
    if (fs.existsSync(file)) {
      return require(file);
    }
  }
  
  // Try node-gyp-build
  try {
    return require('node-gyp-build')(path.join(__dirname));
  } catch (e) {
    // Continue to fallback
  }
  
  // Try standard build location
  const buildPath = path.join(__dirname, 'build', 'Release', 'xinput.node');
  if (fs.existsSync(buildPath)) {
    return require(buildPath);
  }
  
  return null;
}

try {
  binding = tryLoad();
  
  if (!binding) {
    console.error('xinput-napi: No native module found');
    // Return stub module on failure
    binding = {
      getControllerState: () => ({ connected: false }),
      getConnectedControllers: () => [],
      setVibration: () => false,
      isAvailable: () => false,
      getBatteryInfo: () => ({})
    };
  }
} catch (err) {
  console.error('xinput-napi: Failed to load native module:', err.message);
  binding = {
    getControllerState: () => ({ connected: false }),
    getConnectedControllers: () => [],
    setVibration: () => false,
    isAvailable: () => false,
    getBatteryInfo: () => ({})
  };
}

module.exports = binding;

