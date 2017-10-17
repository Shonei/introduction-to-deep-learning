import React, { Component } from 'react';
import {Row, Col, Breadcrumb, MenuItem} from 'react-materialize';

const p = {
  fontSize: '20px'
};

const img = {
  maxHight: '60px',
  maxWidth: '60px'
};

class Lead extends Component {
  constructor(props) {
    super(props);

    this.state = {
      users: <div></div>
    };

    this.componentDidMount = this.componentDidMount.bind(this);
  }

  componentDidMount() {
    this.props.database.ref('scrabble/members').once('value').then(snapshot => {
      const arr = [];
      let place = 1;
      for(let key in snapshot.val()) {
        const temp =  
          <Col key={key} s={12}>
            <p style={p}><img style={img} src={snapshot.val()[key].img} alt={key}/>
            <b>{place}st. </b>Shonei</p>
          </Col>;

        arr.push(temp);
        place++;
      }

      this.setState({users:arr});
    });
  }

  render() {
    return (
      <div>
        <Breadcrumb>
          <MenuItem href="/">Home page</MenuItem>
          <MenuItem href="/scrabble">Scrabble</MenuItem>
          <MenuItem href="/scrabble/leaderboard">Leaderboard</MenuItem>
        </Breadcrumb>
        <div className="container">
          <h3 className="center-align">Leaderboard</h3>
          <Row>
            {this.state.users}
          </Row>
        </div>
      </div>
    );
  }
}

export default Lead;