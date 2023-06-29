const Express = require("express");
const MySqlServer = require("./MysqlConnectionDriver");
const BodyParser = require("body-parser");
const Cors = require("cors");
const { spawn } = require("child_process");
const { stderr } = require("process");
const { error } = require("console");

//Invoke HTTP Server
//HTTPServer();
let LogINResponse = {
  IP: null,
  PORT: null,
  STATUS_CODE: null,
};
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
            console.log( result.IP,result.PORT,result.isRunning);
            if (result.isRunning == 0) {
              console.log("Trying to open a App server instance");
              OpenAddress(result.IP, result.PORT);
            } 
            else
            {
              console.log(
                `Application server is already running on Address ${result.IP}:${result.PORT}`
              );
            }

            //populate the data upon successfull login either with newly created instance or
            //existing instance
            //USE 100 for login success

            PopulateLogInResponse(result.IP, result.PORT, 100);
            ////////////////////////////////////////////////////////
            res.json(LogINResponse);
          })
          .catch((error) => {
            console.log("Error fetching IP");
            //USE -100 for login failure
            PopulateLogInResponse(null, null, -100);
            res.json(LogINResponse);
          });
      } else {
        console.log("Please Register by going to registration page");
        //USE 101 for indicating user to register
        PopulateLogInResponse(null, null, 101);
        res.json(LogINResponse);
      }
    })
    .catch((error) => {
      console.log(error);
      //login failure
      PopulateLogInResponse(null, null, -100);
      res.json(LogINResponse);
    });
});
//opens an Application server in the given IP and port for players to connect
function OpenAddress(IP, PORT) {
  //D:\GameConfigurator\WindowsServer\FlybotServer.exe
  //FlybotServer.exe 127.0.0.1 port=7778 -log
  const ApplicationServerInstance = spawn(`./WindowsServer\\FlybotServer.exe`,[IP,`port=${PORT}`,'-log'],{
    detached:true,
    stdio:'ignore'
  });
  ApplicationServerInstance.unref();
  console.log("Successfully initialized the gameapplication server");
  // Update the server state to running
  MySqlServer.ExecuteQuery(
    `Update companytable set isRunning = ${1} where IP=\'${IP}\' and port=\'${PORT}\'`,
    MySqlServer.RowVolumeEnum.R_ONE
  )
  .then((res)=>{
    console.log('successfully updates server status');
    MySqlServer.CloseServer();
  })
  .catch(error=>{
    console.log('failed to update server status',error);
  });
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

Application.post("/api/RegisterUser", (req, res) => {
  console.log("Register API endpoint hit");
  const UserID = req.body.UserID;
  const Password = req.body.Password;
  const Company = req.body.CompanyID;
  MySqlServer.CreateMySqlConnection();

  MySqlServer.ExecuteQuery(
    `select * from usertable where ID=\'${UserID}\' and Password=\'${Password}\'`,
    MySqlServer.RowVolumeEnum.R_ONE
  ).then((Result) => {
    if (Result != null || Result != undefined) {
      console.log("User already exists , please go to login page");
    } else {
      MySqlServer.ExecuteQuery(
        `insert into usertable(ID,Password,CompanyID) values(\'${UserID}\',\'${Password}\',\'${Company}\')`,
        MySqlServer.RowVolumeEnum.R_ONE
      )
        .then((Res) => {
          console.log("User successfully registerd");
        })
        .catch((Error) => {
          console.log("Unable to register user due to: ", Error);
        });
    }
    MySqlServer.CloseServer();
  });
});

function PopulateLogInResponse(IP, PORT, STATUS_CODE) {
  LogINResponse.IP = IP;
  LogINResponse.PORT = PORT;
  LogINResponse.STATUS_CODE = STATUS_CODE;
}

//This api should be called prior to Shutting down the current server instance 
//This will update the running status of the server  for the concerned company ID
Application.post("/api/UpdateRunningStatus", (req, res) => {
  const CompanyID = req.body.CompanyID;
  MySqlServer.CreateMySqlConnection();
  MySqlServer.ExecuteQuery(
    `update companytable set isRunning=\'${0}\' where CompanyID = CompanyID`,
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
