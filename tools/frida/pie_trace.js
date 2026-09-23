// Traces the interaction menu builder (0xa0621f50, thiscall(objectId, second)) and the
// interaction list query inside it (UI bridge slot 16, (person, object, 0xdacfb8d5, &out)).
Interceptor.attach(va(0xa0621f50), {
    onEnter(args) {
        const sp = this.context.esp;
        send(`pie builder this=${this.context.ecx} id=${sp.add(4).readU32()} second=${sp.add(8).readU32()}`);
    },
    onLeave(r) { send('pie builder returned ' + r); }
});
