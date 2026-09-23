// Traces cGZWinBtn mouse down/up and the click notification (build 1.18.0.187).
const T = {
  'down': 0xa09357ef, 'up': 0xa09359f1, 'notify': 0xa0936f30, 'capture?': 0xa0930f07,
};
for (const [name, v] of Object.entries(T)) {
  Interceptor.attach(va(v), {
    onEnter(args) { this.a = [args[0], args[1], args[2]].map(x => x.toString()); this.ecx = this.context.ecx; },
    onLeave(ret) { send(`${name} this=${this.ecx} args=${this.a} -> ${ret}`); },
  });
}
