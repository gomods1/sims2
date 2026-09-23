// Which parts of the move-in tool's click handler (0xa068858d) run.
const targets = {
    'a079a1d5': 'dispatch', 'a0687a3b': 'moveInMsg', 'a068858d': 'down', 'a079a5a4': 'lotAtScreen',
    'a0687aad': 'fn_7aad', 'a0689c21': 'fn_9c21', 'a068968b': 'fn_968b', 'a068833d': 'fn_833d',
    'a01c9842': 'fn_9842', 'a01c95cf': 'fn_95cf', 'a01c95ec': 'fn_95ec', 'a0689710': 'fn_9710',
    'a0688aba': 'fn_8aba', 'a079a255': 'baseDown', 'a0688bd1': 'fn_8bd1',
};
for (const [a, n] of Object.entries(targets)) {
    try {
        Interceptor.attach(va('0x' + a), {
            onEnter(args) { this.n = n; send(n + ' enter a0=' + args[0] + ' a1=' + args[1] + ' a2=' + args[2]); },
            onLeave(r) { send(n + ' leave ' + r); }
        });
    } catch (e) { send('cannot hook ' + n + ': ' + e); }
}
