export default function Index() {
  const projectDir = process.cwd()
  return <h1>{projectDir}</h1>;
}
