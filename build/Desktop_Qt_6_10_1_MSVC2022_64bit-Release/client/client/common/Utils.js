function isCorrected(isLoginMode, login_, password_1, password_2) {
    if (!login_.trim()) {
        console.log("Empty login");
        errorLabel.text = "Empty login";
        return false;
    }
    if (!password_1.trim()) {
        console.log("Empty password");
        errorLabel.text = "Empty password";
        return false;
    }

    if (!isLoginMode) {
        if (!password_2.trim()) {
            errorLabel.text = "Empty copied password";
            return false;
        }

        if (password_1 !== password_2) {
            errorLabel.text = "Passwords dont match";
            return false;
        }
    }
    return true;
}


