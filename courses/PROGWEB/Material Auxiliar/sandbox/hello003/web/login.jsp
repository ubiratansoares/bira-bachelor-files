<p>Authentication required</p>

<form method="POST" action="index.jsp">
	<p>Username: <input type="text" name="username"></p>
	<p>Password: <input type="password" name="password"></p>
	<input type="submit" value="Login">
</form>

<p>Create New Account</p>

<form method="POST" action="account.jsp">
	<input type="hidden" name="action" value="create">
	<p>Username: <input type="text" name="username"></p>
	<p>Password: <input type="password" name="password"></p>
	<p>E-Mail: <input type="text" name="email"></p>
	<input type="submit" value="Create">
</form>
