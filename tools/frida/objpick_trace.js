// What the tools' object pick (cTSUserToolBase slot 29, 0xa078d789) returns under the cursor.
const seen = {};
Interceptor.attach(va(0xa078d789), {
    onEnter(args) { this.sx = args[0].toInt32(); this.sy = args[1].toInt32(); this.self = this.context.ecx; },
    onLeave(r) {
        const k = r.toString();
        if (seen[k]) return;
        seen[k] = 1;
        let vt = '?';
        try { vt = where(r.readPointer()); } catch (e) { }
        send('pick at ' + this.sx + ',' + this.sy + ' by ' + this.self + ' -> ' + r + ' vt ' + vt + ' from ' + where(this.returnAddress));
    }
});
