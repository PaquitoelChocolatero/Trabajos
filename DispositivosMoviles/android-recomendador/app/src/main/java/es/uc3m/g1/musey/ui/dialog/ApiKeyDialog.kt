package es.uc3m.g1.musey.ui.dialog

import android.app.Dialog
import android.os.Bundle
import androidx.appcompat.app.AlertDialog
import androidx.fragment.app.DialogFragment
import androidx.lifecycle.ViewModelProvider
import es.uc3m.g1.musey.R
import es.uc3m.g1.musey.databinding.DialogApikeyBinding
import es.uc3m.g1.musey.viewModel.MainViewModel

class ApiKeyDialog : DialogFragment() {

    private lateinit var viewModel: MainViewModel

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        val activity =
            this.activity ?: throw java.lang.IllegalStateException("Activity cannot be null")
        viewModel = ViewModelProvider(activity).get(MainViewModel::class.java)

        return activity.let {
            val builder = AlertDialog.Builder(it)

            // Get the layout inflater
            val inflater = requireActivity().layoutInflater
            val binding = DialogApikeyBinding.inflate(inflater)
            val view = binding.root
            // Inflate and set the layout for the dialog
            // Pass null as the parent view because its going in the dialog layout
            builder.setView(view)
                // Add action buttons
                .setPositiveButton(R.string.accept) { _, _ ->
                    viewModel.repository.key = (binding.textApikey.text.toString())
                }
                .setNegativeButton(R.string.cancel) { dialog, _ ->
                    dialog.cancel()
                }

            builder.create()
        }
    }
}