import express from "express";
import checkEmployee from "../controllers/auth.js";
import { Constants } from "../constants.js";

/**
 * This script is responsible to setup the routing of our backend services.
 */
const router = express.Router();

// employee endpoint
router.get("/validate", checkEmployee);

// to match any other path, right now only for testing if its connected to DB
router.use("/", (req, res, next) => {
  res.status(Constants.responseHTTP.badRequest).json({
    error: Constants.responseMsg.pageNotFound,
  });
});

export default router;
