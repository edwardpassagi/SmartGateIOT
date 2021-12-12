import { MongoClient } from "mongodb";
import mongoDbAuth from "../sensitiveConstants.js";

var uri =
  `mongodb+srv://${process.env.dbUsername || mongoDbAuth.username}:${
    process.env.dbPassword || mongoDbAuth.password
  }` +
  `@sellify.5znrc.mongodb.net/${process.env.dbName || mongoDbAuth.dbName}` +
  `?retryWrites=true&w=majority`;

var _db;

const mongoUtil = {
  connectToServer: function (callback) {
    MongoClient.connect(uri, function (err, client) {
      _db = client.db(mongoDbAuth.dbName);
      return callback(err);
    });
  },

  getDb: function () {
    return _db;
  },
};

export default mongoUtil;
