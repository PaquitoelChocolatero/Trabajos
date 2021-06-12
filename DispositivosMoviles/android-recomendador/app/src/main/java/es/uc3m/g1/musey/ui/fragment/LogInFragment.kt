package es.uc3m.g1.musey.ui.fragment

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.widget.Toolbar
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.navigation.findNavController
import androidx.navigation.fragment.findNavController
import com.google.android.gms.auth.api.signin.GoogleSignIn
import com.google.android.gms.auth.api.signin.GoogleSignInClient
import com.google.android.gms.auth.api.signin.GoogleSignInOptions
import com.google.android.gms.common.api.ApiException
import com.google.firebase.auth.GoogleAuthProvider
import es.uc3m.g1.musey.R
import es.uc3m.g1.musey.ServicesDirections
import es.uc3m.g1.musey.databinding.FragmentLoginBinding
import es.uc3m.g1.musey.viewModel.MainViewModel

class LogInFragment : Fragment() {

    companion object {
        private const val RC_SIGN_IN = 120
    }

    private lateinit var viewModel: MainViewModel
    private lateinit var googleSignInClient: GoogleSignInClient

    private lateinit var binding: FragmentLoginBinding


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val navController = findNavController()

        val gso = GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
            .requestIdToken(getString(R.string.default_web_client_id))
            .requestEmail()
            .build()

        viewModel = ViewModelProvider(
            activity ?: throw IllegalStateException("Activity cannot be null")
        ).get(MainViewModel::class.java)

        googleSignInClient = activity?.let {
            GoogleSignIn.getClient(it, gso)
        } ?: throw IllegalStateException(
            "Activity cannot be null"
        )

        if (viewModel.firebase.user != null) {
            navController.navigate(ServicesDirections.login())
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentLoginBinding.inflate(inflater, container, false)

        val toolbar: Toolbar = activity?.findViewById<View>(R.id.toolbar) as Toolbar
        toolbar.title = getString(R.string.login)

        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {

        if (viewModel.firebase.user != null) {
            view.findNavController().navigate(ServicesDirections.login())
        } else {
            super.onViewCreated(view, savedInstanceState)
            with(binding) {
                signInBtnGoogle.setOnClickListener {
                    signIn()
                }
            }
        }
    }

    private fun signIn() {
        val signInIntent = googleSignInClient.signInIntent
        startActivityForResult(signInIntent, RC_SIGN_IN)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        // Result returned from launching the Intent from GoogleSignInApi.getSignInIntent(...);
        if (requestCode == RC_SIGN_IN) {
            val task = GoogleSignIn.getSignedInAccountFromIntent(data)
            val exception = task.exception
            if (task.isSuccessful) {
                try {
                    // Google Sign In was successful, authenticate with Firebase
                    val account = task.getResult(ApiException::class.java)!!
                    Log.d("LogInFragment", "firebaseAuthWithGoogle:" + account.id)
                    firebaseAuthWithGoogle(account.idToken!!)
                } catch (e: ApiException) {
                    // Google Sign In failed, update UI appropriately
                    Log.w("LogInFragment", "Google sign in failed", e)
                }
            } else {
                Log.w("LogInFragment", exception.toString())
            }
        }
    }

    private fun firebaseAuthWithGoogle(idToken: String) {
        val credential = GoogleAuthProvider.getCredential(idToken, null)
        viewModel.firebase.signInWithCredential(credential)
            .addOnCompleteListener(
                activity ?: throw IllegalStateException(
                    "Activity cannot be null"
                )
            ) { task ->
                if (task.isSuccessful) {
                    // Sign in success, update UI with the signed-in user's information
                    Log.d("LogInFragment", "signInWithCredential:success")
                    binding.root.findNavController().navigate(ServicesDirections.login())
                } else {
                    // If sign in fails, display a message to the user.
                    Log.w("LogInFragment", "signInWithCredential:failure", task.exception)
                }
            }
    }
}