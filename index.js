const Express = require("express");
const MySqlServer = require("./MysqlConnectionDriver");
const BodyParser = require("body-parser");
const Cors = require("cors");
const { exec } = require("child_process");
const { stderr } = require("process");
const { error } = require("console");

//Invoke HTTP Server
//HTTPServer();

const Application = Express();
Application.listen(3000, () => {
  console.log("Server is running on port 3000");
});
Application.use(BodyParser.json());
Application.use(Express.json());
Application.use(Cors());

Application.post("/api/LogIn", (req, res) => {
  console.log("LogIN end point hit");
  //res.json({message:`message is ${Option}`});
  //Option -> UserID,Password,Company
  const UserID = req.body.UserID;
  const Password = req.body.Password;
  //Creates a  mysql connection
  MySqlServer.CreateMySqlConnection();
  //Execute query is an asynchronous operation which will return a promise object and we will
  // use the promise object to get the value we need
  let QueryResult = null;
  MySqlServer.ExecuteQuery(
    `select * from usertable where ID=\'${UserID}\' and Password=\'${Password}\'`,
    MySqlServer.RowVolumeEnum.R_ONE
  )
    .then((result) => {
      //console.log(result);
      if (result != null || result != undefined) {
        MySqlServer.ExecuteQuery(
          `select * from companytable where CompanyID=\'${result.CompanyID}\'`,
          MySqlServer.RowVolumeEnum.R_ONE
        )
          .then((result) => {
            //console.log(result);
            //console.log( result.IP,result.PORT);

            OpenAddress(result.IP, result.PORT);
          })
          .catch((error) => {
            console.log("Error fetching IP");
          });
      } else {
        console.log("Please Register by going to registration page");
      }
    })
    .catch((error) => console.log(error));
});
//opens an Application server in the given IP and port for players to connect
function OpenAddress(IP, PORT) {
  //D:\GameConfigurator\WindowsServer\FlybotServer.exe
  //FlybotServer.exe 127.0.0.1 port=7778 -log
  exec(
    `start ./WindowsServer\\FlybotServer.exe ${IP} port=${PORT} -log`,
    (error, stdout, stderr) => {
      if (error) {
        console.log("Error opening the application server ");
      } else {
        console.log("Successfully initialized the gameapplication server");
      }
    }
  );
  MySqlServer.CloseServer();
}
//Get All companies which have registered in our server
Application.get("/api/GetCompanies", (req, res) => {
  console.log("Get companies endpoint hit");
  MySqlServer.CreateMySqlConnection();
  MySqlServer.ExecuteQuery(
    `select CompanyID from companytable`,
    MySqlServer.RowVolumeEnum.R_ALL
  ).then((Result) => {
    console.log(Result);
    // for (Element of Res) {
    //   console.log(Element.CompanyID);
    // }
    res.json(Result);
    MySqlServer.CloseServer();
  });
});

Application.post("/api/RegisterUser",(req,res)=>{
  console.log('Register API endpoint hit');
  const UserID = req.body.UserID;
  const Password = req.body.Password;
  const Company = req.body.CompanyID;
  MySqlServer.CreateMySqlConnection();

  MySqlServer.ExecuteQuery(
    `select * from usertable where ID=\'${UserID}\' and Password=\'${Password}\'`,
    MySqlServer.RowVolumeEnum.R_ONE
  ).then(Result=>{
    if (Result != null || Result != undefined)
    {
        console.log('User already exists , please go to login page');
    }
    else
    {
      MySqlServer.ExecuteQuery(
        `insert into usertable(ID,Password,CompanyID) values(\'${UserID}\',\'${Password}\',\'${Company}\')`,
        MySqlServer.RowVolumeEnum.R_ONE)
      .then(Res=>{
        console.log("User successfully registerd");
      })
      .catch(Error=>{
        console.log('Unable to register user due to: ', Error);
      });
    }
    MySqlServer.CloseServer();
  });

});
