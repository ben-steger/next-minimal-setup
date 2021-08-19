export default function Index() {
  //const projectDir = process.cwd()
  const projUser = process.env.USER;
  return <h1>{projUser}</h1>;
}
