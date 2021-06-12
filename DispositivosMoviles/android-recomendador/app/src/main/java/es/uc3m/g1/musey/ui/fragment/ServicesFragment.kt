package es.uc3m.g1.musey.ui.fragment

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.widget.Toolbar
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import es.uc3m.g1.musey.R
import es.uc3m.g1.musey.databinding.FragmentServicesBinding
import es.uc3m.g1.musey.viewModel.MainViewModel

class ServicesFragment : Fragment() {

    private lateinit var binding: FragmentServicesBinding
    private lateinit var viewModel: MainViewModel

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
        binding = FragmentServicesBinding.inflate(inflater, container, false)

        val toolbar: Toolbar = activity?.findViewById<View>(R.id.toolbar) as Toolbar
        toolbar.title = getString(R.string.services)

        return binding.root
    }
}