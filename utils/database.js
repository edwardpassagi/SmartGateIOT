import { MongoClient } from "mongodb";
import dotenv from "dotenv";
dotenv.config();

var uri =
  `mongodb+srv://${process.env.dbUsername}:${process.env.dbPassword}` +
  `@sellify.5znrc.mongodb.net/${process.env.dbName}` +
  `?retryWrites=true&w=majority`;

var _db;

const mongoUtil = {
  connectToServer: function (callback) {
    MongoClient.connect(uri, function (err, client) {
      _db = client.db(process.env.dbName);
      return callback(err);
    });
  },

  getDb: function () {
    return _db;
  },
};

export default mongoUtil;
