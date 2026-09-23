// Placement tool trace: the mouse-move handler, the world check, the over-lot setters, the verdict.
function f32(p) { return p.readFloat().toFixed(2); }
Interceptor.attach(va(0xa078eb62), {
    onEnter(args) { this.self = this.context.ecx; this.pos = args[1]; send('move handler ' + where(this.returnAddress) + ' screen ' + this.pos.readS32() + ',' + this.pos.add(4).readS32()); },
    onLeave() { send('  after: overlot ' + this.self.add(0x50).readU8() + ' pick ' + f32(this.self.add(0x54)) + ',' + f32(this.self.add(0x58)) + ' L' + this.self.add(0x5c).readS32()); }
});
Interceptor.attach(va(0xa067f31a), { onEnter(args) { send('verdict ' + args[0].toInt32() + ' from ' + where(this.returnAddress)); } });
Interceptor.attach(va(0xa067d331), { onEnter() { send('overlot=1'); } });
Interceptor.attach(va(0xa06815d7), { onEnter(args) { send('slot71 check ' + where(this.returnAddress)); }, onLeave(r) { send('  slot71 -> ' + r.toInt32()); } });
