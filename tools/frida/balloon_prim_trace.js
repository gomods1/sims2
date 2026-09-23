// The balloon/headline primitive (opcode 0x70, handler 0xa05805c0) and the balloon painter:
// the running object, the arguments, and the painted object.
let n = 0;
Interceptor.attach(va(0xa05805c0), {
    onEnter(args) {
        if (n++ > 30) return;
        const self = this.context.ecx;
        let cls = '?';
        send('prim70 this=' + self + ' a0=' + args[0] + ' a1=' + args[1] + ' from ' + where(this.returnAddress));
        try {
            const words = [];
            for (let i = 0; i < 16; i++) words.push(args[1].add(i * 4).readU32().toString(16));
            send('  a1 words: ' + words.join(' '));
        } catch (e) { }
    }
});
Interceptor.attach(va(0xa06c1092), {
    onEnter(args) {
        const obj = args[1];
        let guid = '?';
        try { guid = new NativeFunction(obj.readPointer().add(0x70).readPointer(), 'uint32', ['pointer'], 'thiscall')(obj).toString(16); } catch (e) { guid = 'err ' + e; }
        send('paint obj=' + obj + ' guid=' + guid);
    }
});
