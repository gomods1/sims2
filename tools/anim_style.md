# Animation descriptions: style guide

A blind player presses Control+Shift+A on a Sim and hears what its body is doing, from the
animations the game runs on it. The game only has internal names ("a2o-bed-relax-getin-stop");
the mod speaks a short description instead: "Carlos: getting into bed". These files hold those
descriptions, for The Sims 1 and The Sims 2 (the two mods share them).

## The input

`extracted/anims/en_in/NNN.tsv`: one key per line, TAB, the games (s1, s2), TAB, up to three of
the game's names it comes from (separated by " | "). The key is the name without who does it and
in plain words. The names' first part says who and with what: `a` adult, `c` child, `t` teen,
`p` toddler, `b` baby, `e` elder, `d` `l` `k` `f` pets (dogs, cats, small animals), `w` and others
special Sims; `a2o` an adult with an object, `a2a` an adult with another adult, `c2k` a child with
a cat... The description never says who (the player knows which Sim it is).

## The output

`extracted/anims/en_out/NNN.tsv`, same number: one line per input key, in the same order, the key
exactly as given, TAB, the description. Every key gets a line. UTF-8, no header.

## How a description reads

- English, lowercase (except names), no full stop, no subject: a short "-ing" phrase of 2 to 8
  words: `relaxing in bed`, `getting into bed`, `chewing food`, `stretching a leg`, `hugging
  someone`, `being hugged, accepting it`.
- Say what the body does, in everyday words a player would use; name the object when the key has
  one (`opening the fridge`, `playing chess`). Keep the game's own ideas: an idle (`idle`) is
  what a Sim does while waiting: `waiting, stretching`, `waiting, shifting weight`.
- Roles in social and two-Sim animations: a word ending in -er does it, -ee receives it (kisser
  / kissee, hugger / huggee, tickler / ticklee, asker, sellee...): `kissing someone`, `being
  kissed`. accept / reject: `..., accepting`, `..., refusing`.
- Phases: `start` / `stop`, `getin` / `getout`, `trans` (a transition between two poses): say
  them only when they change the meaning (`getting into the hot tub`, `getting out of bed`,
  `starting to dance`); a plain loop is the action itself.
- Common abbreviations: soc social, gen generic, wt r / wt l weight on the right / left foot,
  trans transition, react reaction, pos position, sim a / sim b the first / second Sim of a pair,
  med / hi / lo intensity, fk ik jig dummy rig terms (ignore them), npc a service or story Sim.
- When a word is unclear, make the best plausible reading from the other words and the example
  names; never leave raw jargon in the description, never invent a detail the key does not hint.
- `-` alone (a hyphen) for what should not be spoken: blinking (`blink underlay`), a neutral or
  absent face (`no expression`), lip shapes for speech (phonemes), pure rig or technical entries.
- Same key style, same wording: near keys read alike (`playing chess`, `playing chess, winning`).

## Translations

`extracted/anims/tr_in/NNN.tsv` (key TAB English) -> `extracted/anims/tr_out/<lang>/NNN.tsv`
(key TAB translation), the same keys in the same order. Translate the meaning naturally, as a
short phrase in the same form as the English (Spanish `relajándose en la cama`, French `se
détendant au lit` or `se détend au lit`: pick the natural form of that language for "what someone
is doing right now" and keep it the same throughout), without a subject or full stop. Use the
game's own vocabulary for its objects where it has a known one (Sim, hot tub...). Keep the key
untouched.

## Resuming (state on 2026-11-25)

English is done (101 chunks, `extracted/anims/en_out`, checked against the inputs). Translations
run in chunks of 200 (`tr_in/000..073`), three agents per language (chunks 000-024, 025-049,
050-073); started: es, fr, de, it, pt. Not started: nl, da, sv, no, fi, pl, cs, hu, ru, ja, ko,
zh-hans, zh-hant, th. `python tools/anim_keys.py status` shows what is done; an agent given a
range skips the chunks already written, so a cut run is simply launched again with the same range.
The agent prompt: read this guide; language and its output folder `tr_out/<code>`; the chunk range;
skip existing outputs; one Write per chunk; key byte-for-byte, TAB, translation; no scripts or
machine translation. When all are done: `python tools/anim_keys.py merge` (writes `mod/anims` here
and in the Sims 1 mod), build and deploy both mods, check Control+Shift+A in Spanish.

State on 2026-11-26: es, fr, de, it, pt done; nl, da, sv, no, fi, pl, cs, hu partly done and
their missing chunks relaunched (one agent per up to 20 missing chunks; the agent reads two done
chunks of its language first and keeps their form). New languages started with chunks 000-018,
the form fixed in the prompt: ru third person present ("слушает музыку, ..."), ja plain ～ている
("ベッドでくつろいでいる"), ko "~는 중" ("침대에서 쉬는 중"), zh-hans / zh-hant a plain verb phrase
with "，" ("在床上放松" / "在床上放鬆", Taiwan usage), th "กำลัง..." ("กำลังพักผ่อนบนเตียง").
Later agents for those languages read 000.tsv of their language first. Check finished chunks
(keys in order, none empty) before merging: three done chunks had lost one line each.

Game-term pass (2026-11-26): `extracted/anims/glossary.tsv` holds the game's own words in each
language for the things it names (WooHoo, Grim Reaper, PlantSim, Servo, Social Bunny,
Mrs. Crumplebottom, Bigfoot, genie, Elixir of Life, Water Wiggler), taken from its STR# tables
(`tools/dbpf.py str` on `TSData/Res/Text/*.package` and the objects packages, matched by resource
and index across languages). After a language is translated: `anim_keys.py glossin`, an agent does
`extracted/anims/gloss_task.md` for it (writes `gloss_out/<lang>.tsv`, the changed lines only),
then `anim_keys.py glossapply`. Done for the 13 first languages; ru, ja, ko, zh-hans, zh-hant, th
after their chunks.

Done (2026-11-26): all 19 translations complete (15086 lines each), the game-term pass applied to
every language, merged into both mods. Checked against the game's own text where the agents
guessed (`PYTHONIOENCODING=utf-8 python tools/dbpf.py str <EPn>/TSData/Res/Objects/objects.package
<STR# language byte>`): the genie's lamp is the game's "magic lamp" in ja (魔法のランプ), ko (마법
램프), th (ตะเกียงวิเศษ), zh-hans 神灯 with the genie 妖怪王; ru "генератор погоды" for the weather
machine; th spells the Grim Reaper ยมฑูต as the game does; ja keeps one word each for vampire and
werewolf (ヴァンパイア, 狼男; the game uses both forms). For new lines later: `collect`, `chunks`,
`tchunks`, translate only the new chunks, `glossin` / the pass / `glossapply`, `merge`.
