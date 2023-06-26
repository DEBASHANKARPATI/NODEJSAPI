const { json } = require("body-parser");
const Mysql = require("mysql2");
const RowVolumeEnum = {
  R_ONE: 0,
  R_ALL: 1,
};

let Connection = null;
function CreateMySqlConnection() {
  Connection = Mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "DesaniDemo!123",
    database: "archvizglobal",
  });

  Connection.connect((err) => {
    if (err) {
      console.log(`failed to connect to the mysql server logged error ${err}`);
      return;
    }
    console.log("Succesfully connected to the mysql server");
  });
}
function ExecuteQuery(Query, NumberOfRecords) {
  return new Promise((resolve, reject) => {
    if (Connection) {
      Connection.query(Query, (err, res) => {
        if (err) {
          //console.log();
          reject(new Error("Unable to execute the query :"+err));
        }
        //if -1 is being passed it will return all the rows in an array it has fetched
        let QueryResult = null;
        if (res) {
          switch (NumberOfRecords) {
            case RowVolumeEnum.R_ONE:
              QueryResult = res[0];
              break;
            case RowVolumeEnum.R_ALL:
              QueryResult = res;
              break;
          }
        }
        //console.log(QueryResult);
        resolve(QueryResult);
      });
    } else {
      reject(new Error("Connection error"));
    }
  });
}
function CloseServer() {
  Connection.end((err) => {
    if (err) {
      console.log("Connection close error: ", err);
    }
  });
}
module.exports = {
  CreateMySqlConnection,
  ExecuteQuery,
  CloseServer,
  RowVolumeEnum,
};
