// Logs cTSWinProcLotInfoDialog::Show (0xa07b7b27, interface @12 slot 4) arguments and caller.
Interceptor.attach(va(0xa07b7b27), {
    onEnter(args) {
        const sp = this.context.esp;
        send(`Show ecx=${this.context.ecx} a1=${sp.add(4).readU32()} a2=${sp.add(8).readU32()} a3=${sp.add(12).readU32()} from ${where(this.returnAddress)}`);
    }
});
