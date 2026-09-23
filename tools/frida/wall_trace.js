// The wall tool's handlers during a drag: the spot each gets, and the tool's drag state.
function v3(p) { try { return p.readFloat().toFixed(2) + ',' + p.add(4).readFloat().toFixed(2) + ',' + p.add(8).readS32(); } catch (e) { return '?'; } }
function state(t) {
    return 'pos74=' + v3(t.add(0x74)) + ' start84=' + v3(t.add(0x84)) + ' end110=' + v3(t.add(0x110)) +
        ' drag11c=' + t.add(0x11c).readU8() + ' mode78=' + t.add(0x78).readS32() + ' m80=' + t.add(0x80).readS32() +
        ' prev68=' + t.add(0x68).readPointer() + ' f4c=' + t.add(0x4c).readU8();
}
const slots = { 'a06925e0': 'slot55 pos', 'a06921e6': 'slot56 down', 'a06926ae': 'slot57 up', 'a069232c': 'slot58 drag', 'a0691b64': 'preview' };
for (const [a, n] of Object.entries(slots)) {
    Interceptor.attach(va('0x' + a), {
        onEnter(args) { this.t = this.context.ecx; send(n + ' arg ' + v3(args[0]) + ' | ' + state(this.t)); },
        onLeave() { send('  after ' + n + ': ' + state(this.t)); }
    });
}
