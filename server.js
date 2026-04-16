const express = require("express")
const app = express()

let dados = []

app.get("/sensor", (req,res) =>{
    const {temp,hum} = req.query

    if (!temp || !hum) {
        return res.status(400).send("dados invalidos")
    }

    const leitura = {
        temperatura: temp,
        umidade: hum,
        data: new Date()
    }

    dados.push(leitura)

    console.log(leitura)

    return send.ok
})


app.get("/sensor", (req,res) =>{

})
const PORT = process.env.PORT || 3000

