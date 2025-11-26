/**
 * Test script for xinput-napi
 */

const xinput = require('./index');

console.log('xinput-napi Test');
console.log('================\n');

console.log('Platform:', process.platform);
console.log('XInput available:', xinput.isAvailable());
console.log('');

if (!xinput.isAvailable()) {
  console.log('XInput is only available on Windows.');
  console.log('On macOS/Linux, use node-hid for Xbox controller access.');
  process.exit(0);
}

const controllers = xinput.getConnectedControllers();
console.log('Connected controllers:', controllers.length ? controllers : 'None');

if (controllers.length === 0) {
  console.log('\nNo controllers connected.');
  process.exit(0);
}

const idx = controllers[0];
console.log(`\nUsing controller ${idx}`);

const battery = xinput.getBatteryInfo(idx);
console.log('Battery:', battery.type || 'Unknown', '-', battery.level || 'Unknown');

console.log('\nPolling... Press Ctrl+C to exit.\n');

let lastPacket = -1;

setInterval(() => {
  const state = xinput.getControllerState(idx);
  
  if (!state.connected) {
    console.log('Disconnected!');
    return;
  }
  
  const g = state.gamepad;
  if (g.packetNumber === lastPacket) return;
  lastPacket = g.packetNumber;
  
  process.stdout.write('\x1B[2J\x1B[0;0H');
  
  console.log('xinput-napi Test - Live Input');
  console.log('=============================\n');
  
  console.log('STICKS:');
  console.log(`  Left:  X=${g.leftStick.x.toFixed(2).padStart(6)} Y=${g.leftStick.y.toFixed(2).padStart(6)}`);
  console.log(`  Right: X=${g.rightStick.x.toFixed(2).padStart(6)} Y=${g.rightStick.y.toFixed(2).padStart(6)}`);
  console.log('');
  
  console.log('TRIGGERS:');
  console.log(`  LT: ${g.leftTrigger.toFixed(2).padStart(5)}  RT: ${g.rightTrigger.toFixed(2).padStart(5)}`);
  console.log('');
  
  const pressed = [];
  const btns = g.buttons;
  if (btns.a) pressed.push('A');
  if (btns.b) pressed.push('B');
  if (btns.x) pressed.push('X');
  if (btns.y) pressed.push('Y');
  if (btns.leftShoulder) pressed.push('LB');
  if (btns.rightShoulder) pressed.push('RB');
  if (btns.leftThumb) pressed.push('L3');
  if (btns.rightThumb) pressed.push('R3');
  if (btns.start) pressed.push('Start');
  if (btns.back) pressed.push('Back');
  if (btns.dpadUp) pressed.push('↑');
  if (btns.dpadDown) pressed.push('↓');
  if (btns.dpadLeft) pressed.push('←');
  if (btns.dpadRight) pressed.push('→');
  
  console.log('BUTTONS:');
  console.log(`  ${pressed.length ? pressed.join(', ') : '(none)'}`);
  console.log('');
  console.log('Press Ctrl+C to exit.');
}, 16);

process.on('SIGINT', () => {
  console.log('\nDone.');
  process.exit(0);
});

