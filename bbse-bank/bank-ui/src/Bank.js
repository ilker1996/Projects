import React, { useEffect, useState } from "react";
import Web3 from "web3";

import Button from "@mui/material/Button";
import Box from "@mui/material/Box";
import Typography from "@mui/material/Typography";
import Container from "@mui/material/Container";
import Paper from "@mui/material/Paper";
import TextField from "@mui/material/TextField";
import Input from "@mui/material/Input";
import InputLabel from "@mui/material/InputLabel";
import InputAdornment from "@mui/material/InputAdornment";
import FormControl from "@mui/material/FormControl";
import Card from "@mui/material/Card";
import CardContent from "@mui/material/CardContent";

import BBSEBank from "./abis/BBSEBank.json";
import BBSEToken from "./abis/BBSEToken.json";
import ETHBBSEPriceFeedOracle from "./abis/ETHBBSEPriceFeedOracle.json";

function Bank() {
  // Input field state vars
  const [newDeposit, setNewDeposit] = useState();
  const [newLoan, setNewLoan] = useState();
  const [newAllowance, setNewAllowance] = useState();

  // General state vars
  const [web3, setWeb3] = useState();
  const [blockNumber, setBlockNumber] = useState();

  // Contract vars
  const [bbseBank, setBankContract] = useState();
  const [bbseToken, setTokenContract] = useState();

  // User related state vars
  const [account, setAccount] = useState();
  const [ethBalance, setEthBalance] = useState();
  const [bbseBalance, setBbseBalance] = useState();
  const [isWalletConnected, setIsWalletConnected] = useState(false);
  const [deposit, setDeposit] = useState();
  const [loan, setLoan] = useState();
  const [bankAllowance, setAllowance] = useState(0);

  // BBSE Bank state vars
  const [avgBlockTime, setAvgBlockTime] = useState(0);
  const [yearlyReturnRate, setYearlyReturnRate] = useState(0);
  const [collateralizationRate, setCollateralizationRate] = useState(0);
  const [loanFee, setLoanFee] = useState(0);
  const [ips, setIps] = useState(0);
  const [minDeposit, setMinDeposit] = useState(0);
  const [ethBBSERate, setETHBBSERate] = useState(0);

  /*   Check MetaMask exists and user account connected
       Initialize state variables
  */
  const checkWalletConnection = async () => {
    if (window.ethereum !== undefined) {
      const web3 = new Web3(window.ethereum);
      const netId = await web3.eth.net.getId();
      // Accounts available on MetaMask should be accesible
      // if MetaMask is connected
      const accounts = await web3.eth.getAccounts();
      // If account state var not initialized and MetMask connected
      if (!account && accounts[0]) {
        // Initialize web3
        setWeb3(web3);
        setBlockNumber(await web3.eth.getBlockNumber());

        // Initalize contracts
        const bbseTokenContract = new web3.eth.Contract(
          BBSEToken.abi, // Contract ABI
          BBSEToken.networks[netId].address // Contract address
        );
        setTokenContract(bbseTokenContract);
        const oracleContract = new web3.eth.Contract(
          ETHBBSEPriceFeedOracle.abi, // Contract ABI
          ETHBBSEPriceFeedOracle.networks[netId].address // Contract address
        );
        // Initialize bank variables
        const bbseBankContract = new web3.eth.Contract(
          BBSEBank.abi, // Contract ABI
          BBSEBank.networks[netId].address // Contract address
        );
        setBankContract(bbseBankContract);

        // Initialize BBBSE Bank state vars
        setYearlyReturnRate(
          await bbseBankContract.methods.yearlyReturnRate().call()
        );
        setCollateralizationRate(
          await bbseBankContract.methods.COLLATERALIZATION_RATIO().call()
        );
        setLoanFee(await bbseBankContract.methods.LOAN_FEE_RATE().call());
        setIps(
          await bbseBankContract.methods.interestPerSecondForMinDeposit().call()
        );
        setMinDeposit(
          await bbseBankContract.methods.MIN_DEPOSIT_AMOUNT().call()
        );
        setETHBBSERate(await oracleContract.methods.getRate().call());
        setAvgBlockTime(await bbseBankContract.methods.AVG_BLOCK_TIME().call());

        // Initialize user account
        const balance = await web3.eth.getBalance(accounts[0]);
        const tokenBalance = await bbseTokenContract.methods
          .balanceOf(accounts[0])
          .call();
        const allowance = await bbseTokenContract.methods
          .allowance(accounts[0], BBSEBank.networks[netId].address)
          .call();
        setAllowance(allowance);
        setAccount(accounts[0]);
        setEthBalance(web3.utils.fromWei(balance, "ether"));
        setBbseBalance(web3.utils.fromWei(tokenBalance, "ether"));
        const deposit = await bbseBankContract.methods
          .investors(accounts[0])
          .call();
        setDeposit(deposit);
        const loan = await bbseBankContract.methods
          .borrowers(accounts[0])
          .call();
        setLoan(loan);

        setIsWalletConnected(true);
      }
    } else {
      alert("Make sure you have MetaMask installed!");
      return;
    }
  };

  useEffect(() => {
    checkWalletConnection();
  }, []);

  // Make deposit from user account
  const handleDeposit = async () => {
    try {
      await bbseBank?.methods.deposit().send({
        from: account,
        value: web3.utils.toWei(newDeposit, "ether"),
      });
      setDeposit(await bbseBank.methods.investors(account).call());
      setBlockNumber(await web3.eth.getBlockNumber());
      const balance = await web3.eth.getBalance(account);
      const tokenBalance = await bbseToken.methods.balanceOf(account).call();
      setEthBalance(web3.utils.fromWei(balance, "ether"));
      setBbseBalance(web3.utils.fromWei(tokenBalance, "ether"));
      setNewDeposit(0);
    } catch (e) {
      console.log(e);
    }
  };

  // Wihtdraw existing deposit
  const handleWithdraw = async () => {
    try {
      await bbseBank?.methods.withdraw().send({
        from: account,
      });
      setDeposit(await bbseBank.methods.investors(account).call());
      setBlockNumber(await web3.eth.getBlockNumber());
      const balance = await web3.eth.getBalance(account);
      const tokenBalance = await bbseToken.methods.balanceOf(account).call();
      setEthBalance(web3.utils.fromWei(balance, "ether"));
      setBbseBalance(web3.utils.fromWei(tokenBalance, "ether"));
    } catch (e) {
      console.log(e);
    }
  };

  // Take loan from BBSE Bank
  const handleLoan = async () => {
    try {
      await bbseBank?.methods
        .borrow(web3.utils.toWei(newLoan.toString(), "ether"))
        .send({
          from: account,
        });
      setLoan(await bbseBank.methods.borrowers(account).call());
      setBlockNumber(await web3.eth.getBlockNumber());
      const balance = await web3.eth.getBalance(account);
      const tokenBalance = await bbseToken.methods.balanceOf(account).call();
      setEthBalance(web3.utils.fromWei(balance, "ether"));
      setBbseBalance(web3.utils.fromWei(tokenBalance, "ether"));
      // Update allowance state var after loan is taken
      const allowance = await bbseToken.methods
        .allowance(account, bbseBank._address)
        .call();
      setAllowance(allowance);
      setNewLoan(0);
    } catch (e) {
      console.log(e);
    }
  };

  // Pay back existing loan
  const handlePayLoan = async () => {
    try {
      await bbseBank?.methods.payLoan().send({
        value: loan.amount,
        from: account,
      });
      setLoan(await bbseBank.methods.borrowers(account).call());
      setBlockNumber(await web3.eth.getBlockNumber());
      const balance = await web3.eth.getBalance(account);
      const tokenBalance = await bbseToken.methods.balanceOf(account).call();
      setEthBalance(web3.utils.fromWei(balance, "ether"));
      setBbseBalance(web3.utils.fromWei(tokenBalance, "ether"));
    } catch (e) {
      console.log(e);
    }
  };

  // Set allowance from user to BBSE Bank
  const handleApprove = async () => {
    await bbseToken.methods
      .approve(
        bbseBank._address,
        web3.utils.toWei(newAllowance.toString(), "ether")
      )
      .send({
        from: account,
      });
    const allowance = await bbseToken.methods
      .allowance(account, bbseBank._address)
      .call();
    setAllowance(allowance);
    setNewAllowance(0);
    setBlockNumber(await web3.eth.getBlockNumber());
  };

  // Handler to connect user MetaMask to app
  const connectWalletHandler = async () => {
    if (window.ethereum !== undefined) {
      try {
        // Request access to MetaMask accounts
        await window.ethereum.request({ method: "eth_requestAccounts" });
        checkWalletConnection();
      } catch (error) {
        console.error(error);
      }
    } else {
      alert("Make sure you have MetaMask installed!");
    }
  };
  // Button to connect user MetaMask to app
  const connectWalletButton = (
    <Box textAlign="center">
      <Button
        variant="contained"
        size="medium"
        onClick={connectWalletHandler}
        style={{ fontWeight: 500 }}
      >
        Connect Wallet
      </Button>
    </Box>
  );

  // User account details component
  const accountInfo = (
    <>
      <FormControl fullWidth style={{ marginTop: 10 }} variant="standard">
        <InputLabel htmlFor="standard-adornment-amount">Address</InputLabel>
        <Input id="standard-adornment-amount" value={account} disabled={true} />
      </FormControl>
      <FormControl fullWidth style={{ marginTop: 10 }} variant="standard">
        <InputLabel htmlFor="standard-adornment-amount">ETH Balance</InputLabel>
        <Input
          id="standard-adornment-amount"
          value={ethBalance}
          startAdornment={<InputAdornment position="start">ETH</InputAdornment>}
          disabled={true}
        />
      </FormControl>
      <FormControl fullWidth style={{ marginTop: 10 }} variant="standard">
        <InputLabel htmlFor="standard-adornment-amount">
          BBSE Token Balance
        </InputLabel>
        <Input
          id="standard-adornment-amount"
          value={bbseBalance}
          startAdornment={
            <InputAdornment position="start">BBSE</InputAdornment>
          }
          disabled={true}
        />
      </FormControl>
    </>
  );

  // Deposit, Loan, Approve intereactions component
  const interactionBoard = (
    <>
      <Typography
        component="h1"
        variant="h5"
        marginBottom={2}
        marginTop={5}
        style={{ fontStyle: "italic" }}
      >
        Deposit
      </Typography>
      {!deposit?.hasActiveDeposit ? (
        <div style={{ marginBottom: 20 }}>
          <div style={{ display: "flex" }}>
            <TextField
              fullWidth
              id="deposit"
              size="small"
              placeholder="Min. 1 ETH"
              name="deposit"
              onChange={(e) => setNewDeposit(e.target.value)}
              value={newDeposit}
              type="number"
              inputProps={{
                step: "0.000001",
                min: 1,
              }}
            />
            <Button
              variant="contained"
              size="small"
              style={{ marginLeft: 10 }}
              onClick={handleDeposit}
              disabled={!(newDeposit >= 1)}
            >
              Deposit
            </Button>
          </div>
          <div style={{ display: "flex", marginTop: 20 }}>
            <Card sx={{ width: 240 }}>
              <CardContent>
                <Typography
                  sx={{ fontSize: 14 }}
                  color="text.secondary"
                  gutterBottom
                >
                  Current Rates & Fees
                </Typography>
                <Typography variant="body2">
                  Yearly Return Rate: {yearlyReturnRate} %
                </Typography>
              </CardContent>
            </Card>
            {newDeposit > 0 ? (
              <Card sx={{ marginLeft: "auto", width: 240 }}>
                <CardContent>
                  <Typography
                    sx={{ fontSize: 14 }}
                    color="text.secondary"
                    gutterBottom
                  >
                    Interest per second
                  </Typography>
                  <Typography variant="body2">
                    {web3.utils.fromWei(
                      parseInt(
                        ips *
                          (web3.utils.toWei(
                            parseFloat(newDeposit).toString(),
                            "ether"
                          ) /
                            parseFloat(minDeposit))
                      ).toString(),
                      "ether"
                    )}{" "}
                    BBSE
                  </Typography>
                </CardContent>
              </Card>
            ) : null}
          </div>
        </div>
      ) : (
        <div style={{ width: "100%", marginBottom: 20 }}>
          <div style={{ display: "flex" }}>
            <Card style={{ marginBottom: 10, marginRight: 10, width: "100%" }}>
              <CardContent>
                <Typography
                  sx={{ fontSize: 14 }}
                  color="text.secondary"
                  gutterBottom
                >
                  Amount{" "}
                </Typography>
                <Typography variant="h5">
                  {web3.utils.fromWei(deposit.amount.toString(), "ether")}
                  ETH
                </Typography>
              </CardContent>
            </Card>
            <Card style={{ marginBottom: 10, width: "100%" }}>
              <CardContent>
                <Typography
                  sx={{ fontSize: 14 }}
                  color="text.secondary"
                  gutterBottom
                >
                  Start Block
                </Typography>
                <Typography variant="h5">{deposit.startTime}</Typography>
              </CardContent>
            </Card>
          </div>
          <Card style={{ marginBottom: 20, width: "100%" }}>
            <CardContent>
              <Typography
                sx={{ fontSize: 14 }}
                color="text.secondary"
                gutterBottom
              >
                Accumulated Interest (based on current known block number)
              </Typography>
              <Typography variant="h5">
                {web3.utils.fromWei(
                  (
                    avgBlockTime *
                    (blockNumber - deposit.startTime + 1) *
                    ips *
                    (deposit.amount / minDeposit)
                  ).toString(),
                  "ether"
                )}{" "}
                BBSE
              </Typography>
            </CardContent>
          </Card>
          <Button
            variant="contained"
            size="small"
            fullWidth
            onClick={handleWithdraw}
          >
            Withdraw
          </Button>
        </div>
      )}

      <hr style={{ marginTop: 40 }}></hr>
      <Typography
        component="h1"
        variant="h5"
        marginBottom={2}
        marginTop={5}
        style={{ fontStyle: "italic" }}
      >
        Loan
      </Typography>
      {!loan?.hasActiveLoan ? (
        <div style={{ marginBottom: 20 }}>
          <div style={{ display: "flex" }}>
            <TextField
              fullWidth
              id="loan"
              size="small"
              placeholder="Loan ETH"
              name="loan"
              onChange={(e) => setNewLoan(e.target.value)}
              value={newLoan}
              type="number"
              inputProps={{
                step: "0.000001",
              }}
            />
            <Button
              variant="contained"
              size="small"
              style={{ marginLeft: 10 }}
              onClick={handleLoan}
              color="secondary"
              disabled={!(newLoan > 0)}
            >
              Borrow
            </Button>
          </div>
          <div style={{ display: "flex", marginTop: 20 }}>
            <Card sx={{ minWidth: 240 }} style={{ marginRight: 10 }}>
              <CardContent>
                <Typography
                  sx={{ fontSize: 14 }}
                  color="text.secondary"
                  gutterBottom
                >
                  Current Rates & Fees
                </Typography>
                <Typography variant="body2">
                  Collateralization Rate: {collateralizationRate} %<br></br>
                  Loan Fee: {loanFee} % <br></br>
                  ETH/BBSE Rate: {ethBBSERate}
                </Typography>
              </CardContent>
            </Card>
            {newLoan > 0 ? (
              <Card sx={{ marginLeft: "auto", width: 240 }}>
                <CardContent>
                  <Typography
                    sx={{ fontSize: 14 }}
                    color="text.secondary"
                    gutterBottom
                  >
                    Approx. Required Min. Collateral
                  </Typography>
                  <Typography variant="body2">
                    {web3.utils.fromWei(
                      parseFloat(
                        (ethBBSERate *
                          (collateralizationRate *
                            web3.utils.toWei(
                              parseFloat(newLoan).toString(),
                              "ether"
                            ))) /
                          100
                      ).toString(),
                      "ether"
                    )}{" "}
                    BBSE
                  </Typography>
                </CardContent>
              </Card>
            ) : null}
          </div>
        </div>
      ) : (
        <div style={{ width: "100%", marginBottom: 20 }}>
          <div style={{ display: "flex" }}>
            <Card style={{ marginBottom: 10, marginRight: 10, width: "100%" }}>
              <CardContent>
                <Typography
                  sx={{ fontSize: 14 }}
                  color="text.secondary"
                  gutterBottom
                >
                  Amount{" "}
                </Typography>
                <Typography variant="h5">
                  {web3.utils.fromWei(loan.amount.toString(), "ether")}
                  ETH
                </Typography>
              </CardContent>
            </Card>
          </div>
          <Card style={{ marginBottom: 20, width: "100%" }}>
            <CardContent>
              <Typography
                sx={{ fontSize: 14 }}
                color="text.secondary"
                gutterBottom
              >
                Collateral
              </Typography>
              <Typography variant="h5">
                {web3.utils.fromWei(loan.collateral.toString(), "ether")} BBSE
              </Typography>
            </CardContent>
          </Card>
          <Button
            variant="contained"
            size="small"
            fullWidth
            color={"secondary"}
            onClick={handlePayLoan}
          >
            Pay Loan
          </Button>
        </div>
      )}

      <hr style={{ marginTop: 40 }}></hr>
      <Typography
        component="h1"
        variant="h5"
        marginBottom={2}
        marginTop={5}
        style={{ fontStyle: "italic" }}
      >
        Approve
      </Typography>
      <div style={{ display: "flex" }}>
        <TextField
          fullWidth
          id="allowance"
          size="small"
          placeholder="Set BBSE allowance of BBSE Bank"
          name="allowance"
          onChange={(e) => setNewAllowance(e.target.value)}
          value={newAllowance}
          type="number"
          inputProps={{
            step: "0.001",
          }}
        />
        <Button
          variant="contained"
          size="small"
          style={{ marginLeft: 10 }}
          onClick={handleApprove}
          color="warning"
          disabled={!(newAllowance > 0)}
        >
          Approve
        </Button>
      </div>
      <Card sx={{ minWidth: "100%", marginTop: 2 }}>
        <CardContent>
          <Typography sx={{ fontSize: 14 }} color="text.secondary" gutterBottom>
            Current Bank Allowance
          </Typography>
          <Typography variant="body2">
            {web3?.utils.fromWei(bankAllowance.toString(), "ether")} BBSE
          </Typography>
        </CardContent>
      </Card>
    </>
  );

  return (
    <Container component="main" maxWidth="sm" sx={{ mb: 4 }}>
      <Paper
        variant="outlined"
        sx={{ my: { xs: 12, md: 6 }, p: { xs: 12, md: 3 } }}
      >
        <Typography
          component="h1"
          variant="h4"
          align="center"
          marginBottom={5}
          fontWeight={600}
        >
          Welcome to BBSEBank
        </Typography>

        {isWalletConnected ? accountInfo : connectWalletButton}
      </Paper>
      {isWalletConnected ? (
        <Paper
          variant="outlined"
          sx={{ my: { xs: 3, md: 6 }, p: { xs: 2, md: 3 } }}
        >
          {interactionBoard}
        </Paper>
      ) : null}
    </Container>
  );
}

export default Bank;
