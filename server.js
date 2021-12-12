import express from "express";
import router from "./routes/routes.js";
import mongoUtil from "./utils/database.js";

/**
 * This script is responsible for our SmartGate's backend API. Routes are
 * defined in routes/routes.js file.
 */

const DEFAULT_PORT = 1880;

mongoUtil.connectToServer(function (err, client) {
  if (err) console.log("Failed to connect to server: ", err);
  const app = express();

  app.use(router);

  app.listen(process.env.PORT || DEFAULT_PORT, function () {
    console.log("Server is running on localhost:", DEFAULT_PORT);
  });
});
