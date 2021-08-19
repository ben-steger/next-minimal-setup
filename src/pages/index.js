export default function Index() {
  //const projectDir = process.cwd()
//   const projUser = process.env.USER;
  let os = require('os')
  return <h1>{os.userInfo().username}</h1>;
}
