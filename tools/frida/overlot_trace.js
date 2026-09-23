// Who clears the object tool's over-lot flag (slot 60, 0xa067d336)?
const seen = {};
Interceptor.attach(va(0xa067d336), { onEnter() {
    const k = stack(this.context, 6);
    seen[k] = (seen[k] || 0) + 1;
    if (seen[k] === 1) send('overlot=0 from ' + k);
} });
Interceptor.attach(va(0xa067d331), { onEnter() { send('overlot=1 from ' + stack(this.context, 4)); } });
