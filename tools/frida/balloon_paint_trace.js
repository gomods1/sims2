// Balloon and headline painting (cTSUIPaintingService slots 6 and 8): what is painted, and who asks.
const seen = {};
function hookIt(addr, name) {
    Interceptor.attach(va(addr), {
        onEnter(args) {
            const k = stack(this.context, 6);
            let extra = '';
            try { extra = ' a0=' + args[0] + ' a1=' + args[1] + ' a2=' + args[2]; } catch (e) { }
            seen[k] = (seen[k] || 0) + 1;
            if (seen[k] <= 2) send(name + extra + ' from ' + k);
        }
    });
}
hookIt('0xa06c1092', 'balloonThumb');
hookIt('0xa06c1917', 'headlineText');
