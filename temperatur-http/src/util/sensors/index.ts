import { readJson, readJsonSync } from 'https://deno.land/x/jsonfile/mod.ts';

const data = await readJson('./data.json') as { name: string; id: number }[];

const content = data.map(d => {
  return {
    filename: d.name.toLowerCase(), text: `char sensor_id[] = "${d.id}"; // get it from stadtpuls.com
char sensor_name[] = "${d.name}";
String path = "/api/v3/sensors/" + String(sensor_id) + "/records";
`};
});


console.log(content);

content.forEach(c => {

  Deno.writeTextFile(`./env_${c.filename}.h`, c.text).then(() => {

    console.log(`env_${c.filename}.h saved!`);
  }).catch((err) => {
    console.error(err);
  })

  // fs.writeFile(`./env_${c.filename}.h`, c.text, "utf-8", function (err) {
  //   if (err) throw err;

  // });
});