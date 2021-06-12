package es.uc3m.g1.musey.ui.fragment

import android.content.res.Resources
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.widget.Toolbar
import androidx.fragment.app.DialogFragment
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import es.uc3m.g1.musey.R
import es.uc3m.g1.musey.ServicesDirections
import es.uc3m.g1.musey.databinding.FragmentProfileBinding
import es.uc3m.g1.musey.talk
import es.uc3m.g1.musey.ui.dialog.KeyPasswordDialog
import es.uc3m.g1.musey.viewModel.MainViewModel
import kotlinx.coroutines.launch
import java.security.InvalidKeyException

class ProfileFragment : Fragment() {
    private lateinit var viewModel: MainViewModel
    private lateinit var binding: FragmentProfileBinding

    private fun DialogFragment.show(tag: String) {
        show(
            this@ProfileFragment.activity?.supportFragmentManager
                ?: throw IllegalStateException(),
            tag
        )
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        viewModel = ViewModelProvider(
            activity ?: throw IllegalStateException("Activity cannot be null")
        ).get(MainViewModel::class.java)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentProfileBinding.inflate(inflater, container, false)

        val toolbar: Toolbar = activity?.findViewById<View>(R.id.toolbar) as Toolbar
        toolbar.title = getString(R.string.profile)

        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val navController = findNavController()

        if (viewModel.firebase.user == null) {
            findNavController().navigate(ServicesDirections.logout())
        }

        with(binding) {
            signOutBtn.setOnClickListener {
                viewModel.firebase.signOut()
                navController.navigate(ServicesDirections.logout())
            }
            backup.setOnClickListener {
                KeyPasswordDialog { password ->
                    try {
                        viewModel.repository.backupApiKey(password)
                        binding.root.talk(R.string.saving_api_key)
                    } catch (e: Resources.NotFoundException) {
                        binding.root.talk(R.string.api_key_error)
                    }
                }.show("backup")
            }
            recover.setOnClickListener {
                KeyPasswordDialog { password ->
                    lifecycleScope.launch {
                        try {
                            if (viewModel.repository.recoverApiKey(password)) {
                                binding.root.talk(R.string.recover_api_key)
                            } else {
                                binding.root.talk(R.string.recover_no_api_key)
                            }
                        } catch (e: InvalidKeyException) {
                            binding.root.talk(R.string.invalid_password)
                        }
                    }
                }.show("recover")
            }
        }
    }
}