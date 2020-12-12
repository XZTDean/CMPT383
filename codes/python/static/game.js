const myAppObj = new Vue({
    el: "#reversi",
    data: {
        gameId: null,
        playersTurn: true,
        game: null,
        board: null,
    },

    methods: {
        newGame: makeNewGame,
        // catMove: sendMoveCats,
        //
        // locationMatches: function(loc, x, y) {
        //     return loc.x == x && loc.y == y;
        // },
    }
});

function makeNewGame() {
    let color = $('input[name=color]:checked').val();
    axios.post('api/games', color, {headers: { 'Content-Type': 'text/plain'}})
        .then(function (response) {
            if (response.status !== 201) {
                console.log("POST new game returned error");
            }
            myAppObj.gameId = response.data
            getGame()
        })
        .catch(function (error) {
            console.log("POST new game error: ", error);
        });
}

function getGame() {
    axios.get('api/games/' + myAppObj.gameId, {})
        .then(function (response) {
            if (response.status === 204) {
                getGame();
                return
            }
            myAppObj.game = response.data
            clearTable()
            makeTable();
        })
        .catch(function (error) {
            console.log("GET game error: ", error);
        });

}

function move(x, y) {
    axios.post('api/games', {
        "x": x,
        "y": y
    })
        .then(function (response) {
            myAppObj.game = response.data
            clearTable()
            makeTable();
            getGame();
        })
        .catch(function (error) {
            if (error.response.status === 400) {
                alert("You can't move there");
                console.log("POST move: Invalid move");
            } else if (error.response.status === 403) {
                alert("It's not your turn");
                console.log("POST move: Invalid turn");
            } else {
                console.log("POST move error: ", error);
            }
        });
}

function deleteGame() {
    axios.delete('api/games/' + myAppObj.gameId, {})
        .then(async function (response) {
            if (response.status !== 204) {
                console.log("POST new game returned error");
            }
            myAppObj.game = null
            myAppObj.gameId = null
            clearTable()
        })
        .catch(function (error) {
            console.log("DELETE game error: ", error);
        });

}

function makeTable() {
    let table = document.getElementById("tableID");
    for (let i = 0; i < 10; i++) {
        let tr = document.createElement('tr');
        for (let j = 0; j < 10; j++) {
            let td = document.createElement('td');
            let img = document.createElement('img');
            myAppObj.game.board[i][j]
            img.src = "/static/" + myAppObj.game.board[i][j] + ".png";
            td.appendChild(img);
            td.onclick = function() {
                move(i - 1, j - 1)
            }
            tr.appendChild(td);
        }
        table.appendChild(tr)
    }
}

function clearTable() {
    let table = document.getElementById("tableID");
    table.innerHTML = "";
}